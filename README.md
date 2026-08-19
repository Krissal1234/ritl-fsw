# RitlFsw

F´ (F Prime) flight software companion to [Rocket-in-the-Loop (RITL)](https://github.com/Krissal1234/rocket_in_the_loop). Built with F Prime plus custom components for parachute deployment and airbrake control, wired up to talk to RITL's `SimBridge` over TCP.

Built against F´ **v4.2.0**.

## Branches

Each branch is a self-contained deployment with a different control-loop scheduling model and starting point:

| Branch | Scheduling | Control logic |
|---|---|---|
| `clock_driven_empty_fsw` | `FlightMain` runs on a fixed-rate timer (rate group), independent of sensor arrival | Skeleton — no control logic filled in |
| `clock_driven_example_fsw` | Same, timer-driven | Reference airbrake + parachute deployment implementation |
| `sensor_driven_empty_fsw` | `FlightMain` runs directly off each incoming sensor packet | Skeleton — no control logic filled in |
| `sensor_driven_example_fsw` | Same, sensor-driven | Reference airbrake + parachute deployment implementation |

All four branches implement the same airbrake and parachute (drogue/main) deployment interfaces. The difference is purely *when* the control loop runs and whether it's pre-filled. Start from an `empty` branch to write your own control logic against a working scaffold, or `example` to see a working reference / use as a baseline to modify.

## Connecting to RITL

This deployment talks to RITL's `SimBridge` over two TCP links, handled by two components present on **all four branches**:

| Component | Role | Port |
|---|---|---|
| `OrchReceiver` | TCP server — accepts RITL's sensor stream, ACKs each packet, forwards it into the deployment | `50100` |
| `OrchSender` | TCP client — connects out to RITL's actuation server and sends actuation commands | `50101` |

These two components are the actual integration point with RITL: they're what every branch has in common regardless of scheduling model or control logic, so they're the first place to look if you're debugging a connection issue or changing how this deployment integrates with `SimBridge`.

For **SIL** (RITL and this deployment on the same machine), both sides default to loopback — nothing to change. On the RITL side, `configs/default.yaml` should have `network.fsw_host: 127.0.0.1`; on this side, `orchSenderClient.configure("127.0.0.1", ORCH_SENDER_PORT)` in `RitlFsw/SilDeployment/Top/SilDeploymentTopology.cpp` is already set to loopback.

For **HIL** (this deployment on real hardware, RITL on the host machine, connected over Ethernet), point each side at the other's actual IP instead of loopback:
- RITL side: set `network.fsw_host` in `configs/default.yaml` to this board's IP.
- This side: change the `orchSenderClient.configure(...)` call to the host machine's IP.

### Packet structure

Both sides must agree on this wire format exactly — it's not negotiated, just assumed fixed on both ends.

**Sensor packet** (RITL → `OrchReceiver`, 64 bytes): eight big-endian `float64` values, in order:

| Bytes | Field |
|---|---|
| 0–7 | `t` (sim time, s) |
| 8–15 / 16–23 / 24–31 | `accel_x` / `accel_y` / `accel_z` |
| 32–39 | `baro` (Pa) |
| 40–47 / 48–55 / 56–63 | `gyro_x` / `gyro_y` / `gyro_z` |

`OrchReceiver` replies with a single ACK byte (`0x06`) after each packet.

**Actuation command** (`OrchSender` → RITL, 1 or 9 bytes): 1-byte command ID, followed by an 8-byte big-endian `float64` payload only for `AIRBRAKE_SET`.

| Byte 0 (command ID) | Payload |
|---|---|
| `0x01` — drogue fire | none (1 byte total) |
| `0x02` — main fire | none (1 byte total) |
| `0x03` — airbrake set | 8-byte `float64` deployment level, `0.0`–`1.0` (9 bytes total) |

If you change either side's data model (add a sensor, add an actuator like TVC), this table and the corresponding struct packing on both `OrchReceiver`/`OrchSender` and RITL's `SensorData`/`ActuationCommand` need to be updated together, or the two sides will desync.


## Changing the control-loop frequency (clock-driven branches)

On `clock_driven_empty_fsw` / `clock_driven_example_fsw`, `FlightMain` runs on `rateGroup1`, driven off a base timer configured in `RitlFsw/SilDeployment/Main.cpp`:

```cpp
RitlFsw::startRateGroups(Fw::TimeInterval(0, 10000));  // 10,000 µs = 10 ms base tick
```

Each rate group then runs at `base tick / divisor`, where the divisors are set in `RitlFsw/SilDeployment/Top/SilDeploymentTopology.cpp`:

```cpp
Svc::RateGroupDriver::DividerSet rateGroupDivisorsSet{{
    {1, 0},    // rateGroup1 — flightMain lives here
    {10, 0},   // rateGroup2
    {100, 0}   // rateGroup3
}};
```

`flightMain.run` is wired to `rateGroup1` (see `topology.fpp`), so to change how often the control loop runs:

- **Fastest path:** change the `1` in `rateGroup1`'s divisor entry — e.g. `{2, 0}` halves its frequency relative to the base tick.
- **Global change:** change the base interval passed to `startRateGroups(...)` in `Main.cpp` — this scales every rate group proportionally, not just `flightMain`'s.


`sensor_driven_*` branches don't use a rate group for `FlightMain` at all. Its execution frequency there is just however fast RITL is sending sensor packets, so this section doesn't apply to them.

## Building & running

```bash
cd RitlFsw/SilDeployment
fprime-util generate
fprime-util build
fprime-gds   # builds/runs the deployment binary + GDS together
```

Start this before running RITL in `sil` mode. RITL connects to the sensor port on startup.

## Requirements

```bash
pip install -r requirements.txt
```
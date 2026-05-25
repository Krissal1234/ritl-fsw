// ======================================================================
// \title  SilDeploymentTopology.cpp
// \brief cpp file containing the topology instantiation code
//
// ======================================================================
// Provides access to autocoded functions
#include <RitlFsw/SilDeployment/Top/SilDeploymentTopologyAc.hpp>
// Note: Uncomment when using Svc:TlmPacketizer
//#include <RitlFsw/SilDeployment/Top/SilDeploymentPacketsAc.hpp>

// Necessary project-specified types
#include <Fw/Types/MallocAllocator.hpp>
#include <Svc/BufferManager/BufferManager.hpp>
// #include <RitlFsw/Types/TransportTypes.fpp>

// Public functions for use in main program are namespaced with deployment module RitlFsw
// This is also the namespace where the topology components are instantiated by FPP.
namespace RitlFsw {

// Instantiate a malloc allocator for cmdSeq buffer allocation
Fw::MallocAllocator mallocator;

// The reference topology divides the incoming clock signal (1Hz) into sub-signals: 1Hz, 1/2Hz, and 1/4Hz with 0 offset
Svc::RateGroupDriver::DividerSet rateGroupDivisorsSet{{{2, 0}, {10, 0}, {100, 0}}};


// Rate groups may supply a context token to each of the attached children whose purpose is set by the project. The
// reference topology sets each token to zero as these contexts are unused in this project.
U32 rateGroup1Context[Svc::ActiveRateGroup::CONNECTION_COUNT_MAX] = {};
U32 rateGroup2Context[Svc::ActiveRateGroup::CONNECTION_COUNT_MAX] = {};
U32 rateGroup3Context[Svc::ActiveRateGroup::CONNECTION_COUNT_MAX] = {};

enum TopologyConstants {
    COMM_PRIORITY        = 34,
    SENSOR_PRIORITY      = 33,
    ORCH_SENDER_PRIORITY = 32,
    SENSOR_PORT          = 50100,
    ORCH_SENDER_PORT     = 50101,
};

/**
 * \brief configure/setup components in project-specific way
 *
 * This is a *helper* function which configures/sets up each component requiring project specific input. This includes
 * allocating resources, passing-in arguments, etc. This function may be inlined into the topology setup function if
 * desired, but is extracted here for clarity.
 */
void configureTopology() {
    // Rate group driver needs a divisor list
    rateGroupDriver.configure(rateGroupDivisorsSet);

    // Rate groups require context arrays.
    rateGroup1.configure(rateGroup1Context, FW_NUM_ARRAY_ELEMENTS(rateGroup1Context));
    rateGroup2.configure(rateGroup2Context, FW_NUM_ARRAY_ELEMENTS(rateGroup2Context));
    rateGroup3.configure(rateGroup3Context, FW_NUM_ARRAY_ELEMENTS(rateGroup3Context));

    // Command sequencer needs to allocate memory to hold contents of command sequences
    cmdSeq.allocateBuffer(0, mallocator, 5 * 1024);

    // Buffer manager for sensorServer receive buffers: 10 x 1024 bytes
    Svc::BufferManager::BufferBins sensorBins = {};
    sensorBins.bins[0].bufferSize = 1024;
    sensorBins.bins[0].numBuffers = 10;
    sensorBufferManager.setup(1, 0, mallocator, sensorBins);

    // Buffer manager for orchSender:
    //   Bin 0: small buffers for serializing outgoing ActuationCommands (1 byte each)
    //   Bin 1: 1024-byte buffers for orchSenderClient's TCP receive task
    Svc::BufferManager::BufferBins orchSenderBins = {};
    orchSenderBins.bins[0].bufferSize = RitlFsw::ActuationCommand::SERIALIZED_SIZE;
    orchSenderBins.bins[0].numBuffers = 10;
    orchSenderBins.bins[1].bufferSize = 1024;
    orchSenderBins.bins[1].numBuffers = 2;
    orchSenderBufferManager.setup(2, 0, mallocator, orchSenderBins);

}

void setupTopology(const TopologyState& state) {
    // Autocoded initialization. Function provided by autocoder.
    initComponents(state);
    // Autocoded id setup. Function provided by autocoder.
    setBaseIds();
    // Autocoded connection wiring. Function provided by autocoder.
    connectComponents();
    // Autocoded command registration. Function provided by autocoder.
    regCommands();
    // Autocoded configuration. Function provided by autocoder.
    configComponents(state);
    if (state.hostname != nullptr && state.port != 0) {
        comDriver.configure(state.hostname, state.port);
    }
    // Project-specific component configuration. Function provided above. May be inlined, if desired.
    configureTopology();
    // Autocoded parameter loading. Function provided by autocoder.
    loadParameters();
    // Autocoded task kick-off (active components). Function provided by autocoder.
    startTasks(state);
    // Initialize socket communication if and only if there is a valid specification
    if (state.hostname != nullptr && state.port != 0) {
        Os::TaskString name("ReceiveTask");
        // Uplink is configured for receive so a socket task is started
        comDriver.start(name, COMM_PRIORITY, Default::STACK_SIZE);
    }

    // Start sensor TCP server (listens for incoming orchestrator connections)
    sensorServer.configure("0.0.0.0", static_cast<U16>(SENSOR_PORT));
    Os::TaskString sensorTaskName("SensorReceiveTask");
    sensorServer.start(sensorTaskName, SENSOR_PRIORITY, Default::STACK_SIZE);

    // Connect orchSenderClient to the Python actuation server on port 50101.
    // The Python orchestrator is always local (Docker port-mapped to 127.0.0.1),
    // so this address is independent of the GDS hostname in state.hostname.
    orchSenderClient.configure("127.0.0.1", static_cast<U16>(ORCH_SENDER_PORT));
    // orchSenderClient.configure("10.42.0.1", static_cast<U16>(ORCH_SENDER_PORT));
    Os::TaskString orchSenderTaskName("OrchSenderTask");
    orchSenderClient.start(orchSenderTaskName, ORCH_SENDER_PRIORITY, Default::STACK_SIZE);
}

void startRateGroups(const Fw::TimeInterval& interval) {
    // The timer component drives the fundamental tick rate of the system.
    // Svc::RateGroupDriver will divide this down to the slower rate groups.
    // This call will block until the stopRateGroups() call is made.
    // For this Linux demo, that call is made from a signal handler.
    timer.startTimer(interval);
}

void stopRateGroups() {
    timer.quit();
}

void teardownTopology(const TopologyState& state) {
    // Autocoded (active component) task clean-up. Functions provided by topology autocoder.
    stopTasks(state);
    freeThreads(state);

    // Other task clean-up.
    comDriver.stop();
    (void)comDriver.join();

    sensorServer.stop();
    (void)sensorServer.join();

    orchSenderClient.stop();
    (void)orchSenderClient.join();

    // Resource deallocation
    cmdSeq.deallocateBuffer(mallocator);
    sensorBufferManager.cleanup();
    orchSenderBufferManager.cleanup();

    tearDownComponents(state);
}
};  // namespace RitlFsw

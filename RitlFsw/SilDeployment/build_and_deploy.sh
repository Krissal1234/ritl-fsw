#!/bin/bash
PI=${1:-pi@10.42.0.142}
DEPLOY_DIR="/home/pi/"
TOOLCHAIN="../../cmake/toolchain/aarch64-rpi4.cmake"
BINARY="../../build-artifacts/aarch64-rpi4/RitlFsw_SilDeployment/bin/RitlFsw_SilDeployment"

echo "==> Building for RPi4 (aarch64)..."
fprime-util generate --build-cache ../../build-rpi4 -DCMAKE_TOOLCHAIN_FILE=$TOOLCHAIN 2>/dev/null || true
fprime-util build --build-cache ../../build-rpi4

echo "==> Copying to Pi ($PI)..."
ssh $PI "mkdir -p $DEPLOY_DIR"
scp $BINARY $PI:$DEPLOY_DIR/RitlFsw_SilDeployment

echo "==> Done. Run with:"
echo "    ssh $PI '$DEPLOY_DIR/RitlFsw_SilDeployment -a 10.42.0.1 -p 50000'"
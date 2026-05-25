#!/bin/bash
PI=${1:-pi@10.42.0.142}
DEPLOY_DIR="/home/pi/ritl-fsw"
BINARY="../../build-artifacts/Linux/RitlFsw_SilDeployment/bin/RitlFsw_SilDeployment"

echo "==> Building..."
fprime-util generate 2>/dev/null || true
fprime-util build

echo "==> Copying to Pi ($PI)..."
ssh $PI "mkdir -p $DEPLOY_DIR"
scp $BINARY $PI:$DEPLOY_DIR/RitlFsw_SilDeployment

echo "==> Done. Run with:"
echo "    ssh $PI '$DEPLOY_DIR/RitlFsw_SilDeployment -a 10.42.0.1 -p 50000'"
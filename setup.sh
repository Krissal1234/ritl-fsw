#!/bin/bash
set -e

echo "Setting up RITL Skeleton Flight Software..."

git clone --branch v4.2.0 https://github.com/nasa/fprime lib/fprime

python3 -m venv venv
source venv/bin/activate
pip install -r requirements.txt

echo ""
echo "Setup complete! To run the flight software:"
echo ""
echo "  source venv/bin/activate"
echo "  fprime-util generate"
echo "  fprime-util build"
echo "  fprime-gds"
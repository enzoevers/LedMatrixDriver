#!/bin/bash

set -e

function cleanup()
{
    echo
    echo "//===================="
    echo "// Clean targets"
    echo "//===================="
    echo 

    sudo Scripts/Stm32/Stm32f303xc/CleanStm32f303xcBuild.sh
}

trap cleanup EXIT

echo
echo "//===================="
echo "// Install submodules"
echo "//===================="
echo 

git submodule init
git submodule update

echo
echo "//===================="
echo "// Generic installs"
echo "//===================="
echo 

sudo apt install -y clang-format
python3 -m pip install fontTools pillow argparse

echo
echo "//===================="
echo "// stm32 installs"
echo "//===================="
echo 

sudo apt install -y libnewlib-arm-none-eabi

echo
echo "//===================="
echo "// Build STM32F303xc"
echo "//===================="
echo 

Scripts/Stm32/Stm32f303xc/BuildStm32f303xc.sh "build"
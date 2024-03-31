#!/bin/bash

set -e

function cleanup()
{
    echo
    echo "//===================="
    echo "// Clean Doxygen"
    echo "//===================="
    echo 

    Scripts/Doc/CleanDocBuild.sh
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
echo "// Generate doxygen"
echo "//===================="
echo 

Scripts/Doc/BuildDoc.sh
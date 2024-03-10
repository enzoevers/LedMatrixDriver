#!/bin/bash

CUR_PATH=$(dirname $(realpath "$0"))
SCRIPT_PATH=${CUR_PATH}/../

source ${SCRIPT_PATH}/Variables.sh
source ${SCRIPT_PATH}/Utils.sh

ValidateDirExists ${CODE_PATH}

cd ${BUILD_DIR_TEST}

# Run all tests
echo "//========================="
echo "// Running TestHAL"
echo "//========================="
./HAL/test/TestHAL

echo "//========================="
echo "// Running TestDisplays"
echo "//========================="
./Displays/test/TestDisplays
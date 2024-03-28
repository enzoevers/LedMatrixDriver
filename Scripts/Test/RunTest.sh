#!/bin/bash

CUR_PATH=$(dirname $(realpath "$0"))
SCRIPT_PATH=${CUR_PATH}/../

source ${SCRIPT_PATH}/Variables.sh
source ${SCRIPT_PATH}/Utils.sh

ValidateDirExists ${CODE_PATH}

cd ${BUILD_DIR_TEST}

# Run all tests

echo
echo "//========================="
echo "// Running TestHAL"
echo "//========================="
echo

./HAL/test/TestHAL

echo
echo "//========================="
echo "// Running TestDisplays"
echo "//========================="
echo

./Displays/test/TestDisplays
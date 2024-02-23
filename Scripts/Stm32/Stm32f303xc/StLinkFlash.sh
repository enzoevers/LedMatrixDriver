#!/bin/bash

CUR_PATH=$(dirname $(realpath "$0"))
SCRIPT_PATH=${CUR_PATH}/../../

source ${SCRIPT_PATH}/Variables.sh
source ${SCRIPT_PATH}/Utils.sh

ValidateDirExists ${BUILD_DIR_STM32F303XC}

echo "APPLICATION_NAME ${APPLICATION_NAME}"

BINARY_PATH=${BUILD_DIR_STM32F303XC}/${APPLICATION_NAME}.bin
ValidateFileExists ${BINARY_PATH}

WRITE_ADDRESS=0x8000000

st-flash write ${BINARY_PATH} ${WRITE_ADDRESS}
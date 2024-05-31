#!/bin/bash

DO_BUILD="$1"

CUR_PATH=$(dirname $(realpath "$0"))
SCRIPT_PATH=${CUR_PATH}/../../

source ${SCRIPT_PATH}/Variables.sh
source ${SCRIPT_PATH}/Utils.sh

CreateBuildDirectoryIfNotExist ${BUILD_PATH}

ValidateDirExists ${CODE_PATH}

CreateBuildDirectoryIfNotExist ${BUILD_DIR_STM32F303XC}

cd ${BUILD_DIR_STM32F303XC}
cmake -G Ninja ${CODE_PATH} -DCMAKE_BUILD_TYPE=Debug -DUSE_STM32=ON -DUSE_STM32F3=ON -DUSE_STM32F303XC=ON -DHANOVER_OL037A=ON

if [ "${DO_BUILD}" = "build" ]; then
    cmake --build . --parallel 8
fi
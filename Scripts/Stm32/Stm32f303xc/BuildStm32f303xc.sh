#!/bin/bash

CUR_PATH=$(dirname $(realpath "$0"))
SCRIPT_PATH=${CUR_PATH}/../../

source ${SCRIPT_PATH}/Variables.sh
source ${SCRIPT_PATH}/Utils.sh

ValidateDirExists ${CODE_PATH}

CreateBuildDirectoryIfNotExist ${BUILD_DIR_STM32F303XC}

cd ${BUILD_DIR_STM32F303XC}
cmake .. -DUSE_STM32=ON -DUSE_STM32F3=ON -DUSE_STM32F303XC=ON
make -j8

echo "Done"
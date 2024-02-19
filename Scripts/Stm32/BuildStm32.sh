#!/bin/bash

CUR_PATH=$(dirname $(realpath "$0"))
SCRIPT_PATH=${CUR_PATH}/../

source ${SCRIPT_PATH}/Variables.sh
source ${SCRIPT_PATH}/Utils.sh

ValidateCodePath ${CODE_PATH}

CreateBuildDirectoryIfNotExist ${BUILD_DIR_STM32}

cd ${BUILD_DIR_STM32}
cmake .. -DUSE_STM32=ON -DUSE_STM32F303XC=ON
make -j8

echo "Done"
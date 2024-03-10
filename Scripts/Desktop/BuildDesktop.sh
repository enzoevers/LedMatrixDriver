#!/bin/bash

CUR_PATH=$(dirname $(realpath "$0"))
SCRIPT_PATH=${CUR_PATH}/../

source ${SCRIPT_PATH}/Variables.sh
source ${SCRIPT_PATH}/Utils.sh

ValidateDirExists ${CODE_PATH}

CreateBuildDirectoryIfNotExist ${BUILD_DIR_DESKTOP}

cd ${BUILD_DIR_DESKTOP}
cmake -G Ninja .. -DUSE_DESKTOP=ON
cmake --build . --parallel 8
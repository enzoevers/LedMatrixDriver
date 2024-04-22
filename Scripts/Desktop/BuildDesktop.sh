#!/bin/bash

CUR_PATH=$(dirname $(realpath "$0"))
SCRIPT_PATH=${CUR_PATH}/../

source ${SCRIPT_PATH}/Variables.sh
source ${SCRIPT_PATH}/Utils.sh

CreateBuildDirectoryIfNotExist ${BUILD_PATH}

ValidateDirExists ${CODE_PATH}

CreateBuildDirectoryIfNotExist ${BUILD_DIR_DESKTOP}

cd ${BUILD_DIR_DESKTOP}
cmake -G Ninja ${CODE_PATH} -DUSE_DESKTOP=ON -DHANOVER_OL037A=ON
cmake --build . --parallel 8
#!/bin/bash

CUR_PATH=$(dirname $(realpath "$0"))
SCRIPT_PATH=${CUR_PATH}/../

source ${SCRIPT_PATH}/Variables.sh
source ${SCRIPT_PATH}/Utils.sh

ValidateCodePath ${CODE_PATH}

CreateBuildDirectoryIfNotExist ${BUILD_DIR_DESKTOP}

cd ${BUILD_DIR_DESKTOP}
cmake .. -DUSE_DESKTOP=ON
make -j8

echo "Done"
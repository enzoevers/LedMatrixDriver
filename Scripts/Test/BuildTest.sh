#!/bin/bash

CUR_PATH=$(dirname $(realpath "$0"))
SCRIPT_PATH=${CUR_PATH}/../

source ${SCRIPT_PATH}/Variables.sh
source ${SCRIPT_PATH}/Utils.sh

ValidateCodePath ${CODE_PATH}

CreateBuildDirectoryIfNotExist ${BUILD_DIR_TEST}

cd ${BUILD_DIR_TEST}
CC=clang CXX=clang++ cmake .. -DTEST_ON_PC=ON
make -j8

echo "Done"
#!/bin/bash

CUR_PATH=$(dirname $(realpath "$0"))
SCRIPT_PATH=${CUR_PATH}/../

source ${SCRIPT_PATH}/Variables.sh
source ${SCRIPT_PATH}/Utils.sh

ValidateDirExists ${CODE_PATH}

CreateBuildDirectoryIfNotExist ${BUILD_DIR_TEST}

cd ${BUILD_DIR_TEST}
CC=clang CXX=clang++ cmake -G Ninja .. -DTEST_ON_PC=ON
cmake --build .

echo "Done"
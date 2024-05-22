#!/bin/bash

DO_BUILD="$1"

CUR_PATH=$(dirname $(realpath "$0"))
SCRIPT_PATH=${CUR_PATH}/../

source ${SCRIPT_PATH}/Variables.sh
source ${SCRIPT_PATH}/Utils.sh

CreateBuildDirectoryIfNotExist ${BUILD_PATH}

ValidateDirExists ${CODE_PATH}

CreateBuildDirectoryIfNotExist ${BUILD_DIR_TEST}

cd ${BUILD_DIR_TEST}
CC=clang-17 CXX=clang++-17 cmake -G Ninja ${CODE_PATH} -DCMAKE_BUILD_TYPE=Debug -DTEST_ON_PC=ON

if [ "${DO_BUILD}" = "build" ]; then
    cmake --build . --parallel 8
fi
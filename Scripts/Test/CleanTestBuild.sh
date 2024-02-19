#!/bin/bash

CUR_PATH=$(dirname $(realpath "$0"))
SCRIPT_PATH=${CUR_PATH}/../

source ${SCRIPT_PATH}/Variables.sh
source ${SCRIPT_PATH}/Utils.sh

ValidateCodePath ${CODE_PATH}

DeleteBuildDirectoryIfExist ${BUILD_DIR_TEST}

echo "Done"
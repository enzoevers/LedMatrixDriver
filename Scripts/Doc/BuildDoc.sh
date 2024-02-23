#!/bin/bash

CUR_PATH=$(dirname $(realpath "$0"))
SCRIPT_PATH=${CUR_PATH}/../

source ${SCRIPT_PATH}/Variables.sh
source ${SCRIPT_PATH}/Utils.sh

ValidateDirExists ${CODE_PATH}

CreateBuildDirectoryIfNotExist ${BUILD_DIR_DOC}

cd ${BUILD_DIR_DOC}
cmake .. -DGENERATE_DOCS=ON
doxygen Doc/Doxyfile.doxygen

echo "Done"
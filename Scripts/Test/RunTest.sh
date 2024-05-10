#!/bin/bash

CUR_PATH=$(dirname $(realpath "$0"))
SCRIPT_PATH=${CUR_PATH}/../

source ${SCRIPT_PATH}/Variables.sh
source ${SCRIPT_PATH}/Utils.sh

ValidateDirExists ${CODE_PATH}

RETURN_CODE=0
EvaluateTests()
{
    LAST_LINE=$( tail -n 3 $1 | tr '\n' ' ' )
    if [[ $LAST_LINE == *"FAILED"* ]]  || [[ $LAST_LINE != *"PASSED"* && $LAST_LINE != *"DISABLED TEST"* ]]; then
        RETURN_CODE=$((RETURN_CODE | 1))
        echo "RETURN_CODE ${RETURN_CODE}"
    fi
}

# Run all tests

echo
echo "//========================="
echo "// Running TestHAL"
echo "//========================="
echo

${BUILD_DIR_TEST}/HAL/test/TestHAL 2>&1 | tee TestHAL_log.txt
EvaluateTests TestHAL_log.txt
rm TestHAL_log.txt

echo
echo "//========================="
echo "// Running TestDisplays"
echo "//========================="
echo

${BUILD_DIR_TEST}/Displays/test/TestDisplays 2>&1 | tee TestDisplays_log.txt
EvaluateTests TestDisplays_log.txt
rm TestDisplays_log.txt

exit ${RETURN_CODE}
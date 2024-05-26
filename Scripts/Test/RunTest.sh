#!/bin/bash

CUR_PATH=$(dirname $(realpath "$0"))
SCRIPT_PATH=${CUR_PATH}/../

source ${SCRIPT_PATH}/Variables.sh
source ${SCRIPT_PATH}/Utils.sh

ValidateDirExists ${CODE_PATH}
ValidateDirExists ${BUILD_PATH}
ValidateDirExists ${EXTERNAL_PATH}

CreateBuildDirectoryIfNotExist ${COVERAGE_PATH}
ValidateDirExists ${COVERAGE_PATH}

RETURN_CODE=0
EvaluateTests()
{
    LAST_LINE=$( tail -n 3 $1 | tr '\n' ' ' )
    if [[ $LAST_LINE == *"FAILED"* ]]  || [[ $LAST_LINE != *"PASSED"* && $LAST_LINE != *"DISABLED TEST"* ]]; then
        RETURN_CODE=$((RETURN_CODE | 1))
        echo "RETURN_CODE ${RETURN_CODE}"
    fi
}

RunTests()
{
    TEST_NAME=$1
    TEST_PATH=$2
    
    LLVM_PROFILE_FILE=${TEST_PATH}/${TEST_NAME}.profraw ${TEST_PATH}/${TEST_NAME} 2>&1 | tee ${TEST_PATH}/${TEST_NAME}_log.txt
    EvaluateTests ${TEST_PATH}/${TEST_NAME}_log.txt
    rm ${TEST_PATH}/${TEST_NAME}_log.txt
}

TRACE_FILES=()

GenerateTraceFile()
{
    TEST_NAME=$1
    TEST_PATH=$2

    llvm-profdata merge -output=${TEST_PATH}/${TEST_NAME}.profdata ${TEST_PATH}/${TEST_NAME}.profraw
    llvm-cov export -format=lcov ${TEST_PATH}/${TEST_NAME} -instr-profile=${TEST_PATH}/${TEST_NAME}.profdata > ${TEST_PATH}/${TEST_NAME}_Coverage.info
    
    TRACE_FILES+=( "${TEST_PATH}/${TEST_NAME}_Coverage.info" )
}

GenerateCoverageHtml()
{
    OUTPUT_DIRECTORY=$1

    TRACE_FILES_COMMAND=( "${TRACE_FILES[@]/#/'-a '}" )
    TRACE_FILE_COMMAND_STRING=$(printf "%s " "${TRACE_FILES_COMMAND[@]}")

    echo
    echo "//-------------------------"
    echo "// Combine tracefiles"
    echo "//-------------------------"
    echo

    lcov ${TRACE_FILE_COMMAND_STRING} -o ${COVERAGE_PATH}/coverage.info

    SANATIZED_CODE_PATH=$(realpath ${CODE_PATH})

    lcov --remove ${COVERAGE_PATH}/coverage.info \
        "${SANATIZED_CODE_PATH}/**/test/*" \
        "${SANATIZED_CODE_PATH}/**/fuzztest/*" \
        "${SANATIZED_CODE_PATH}/**/HardwareSetup/*" \
        -o ${COVERAGE_PATH}/coverage.info

    lcov --extract ${COVERAGE_PATH}/coverage.info \
        "${SANATIZED_CODE_PATH}/**/*.cpp" \
        "${SANATIZED_CODE_PATH}/**/*.c" \
        "${SANATIZED_CODE_PATH}/**/*.hpp" \
        "${SANATIZED_CODE_PATH}/**/*.h" \
        -o ${COVERAGE_PATH}/coverage.info
    
    echo
    echo "//-------------------------"
    echo "// Generate html"
    echo "//-------------------------"
    echo

    mkdir -p ${OUTPUT_DIRECTORY}
    genhtml -o ${OUTPUT_DIRECTORY} ${COVERAGE_PATH}/coverage.info

    echo
    echo "//-------------------------"
    echo "// Generate badge"
    echo "//-------------------------"
    echo

    cd ${EXTERNAL_PATH}/lcov-badge
    yarn install --non-interactive
    cd ${CUR_PATH}

    SANATIZED_COVERAGE_PATH=$(realpath ${COVERAGE_PATH})
    ${EXTERNAL_PATH}/lcov-badge/lcov-badge -o ${SANATIZED_COVERAGE_PATH}/coverage.svg ${SANATIZED_COVERAGE_PATH}/coverage.info
}

echo
echo "//========================="
echo "// Running TestHAL"
echo "//========================="
echo

RunTests TestHAL ${BUILD_DIR_TEST}/HAL/test/
GenerateTraceFile TestHAL ${BUILD_DIR_TEST}/HAL/test/

echo
echo "//========================="
echo "// Running TestDisplays"
echo "//========================="
echo

RunTests TestDisplays ${BUILD_DIR_TEST}/Displays/test/
GenerateTraceFile TestDisplays ${BUILD_DIR_TEST}/Displays/test/

echo
echo "//========================="
echo "// Running TestTextProvider"
echo "//========================="
echo

RunTests TestTextProvider ${BUILD_DIR_TEST}/TextProvider/test/
GenerateTraceFile TestTextProvider ${BUILD_DIR_TEST}/TextProvider/test/

if [ ${RETURN_CODE} != 0 ]; then
    exit ${RETURN_CODE}
fi

echo
echo "//========================="
echo "// Generating coverage report"
echo "//========================="
echo

GenerateCoverageHtml ${COVERAGE_PATH}/CoverageReport/

exit ${RETURN_CODE}
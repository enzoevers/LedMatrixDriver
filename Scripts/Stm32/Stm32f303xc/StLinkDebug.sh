#!/bin/bash

CUR_PATH=$(dirname $(realpath "$0"))
SCRIPT_PATH=${CUR_PATH}/../../

source ${SCRIPT_PATH}/Variables.sh
source ${SCRIPT_PATH}/Utils.sh

ValidateDirExists ${BUILD_DIR_STM32F303XC}

echo "APPLICATION_NAME ${APPLICATION_NAME}"

ELF_PATH=${BUILD_DIR_STM32F303XC}/${APPLICATION_NAME}.elf

echo
echo "//========================="
echo "// Starting openocd in background"
echo "//========================="
echo

openocd \
    -f /usr/share/openocd/scripts/interface/stlink-v2.cfg \
    -f /usr/share/openocd/scripts/target/stm32f3x.cfg \
    &
OPENOCD_PROC_ID=$!

sleep 3

echo
echo "//========================="
echo "// Starting gdb-multiarch"
echo "//========================="
echo

gdb-multiarch ${ELF_PATH}

kill ${OPENOCD_PROC_ID}
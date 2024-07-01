#!/bin/bash

APP_NAME="$1"

CUR_PATH=$(dirname $(realpath "$0"))
SCRIPT_PATH=${CUR_PATH}/../../

source ${SCRIPT_PATH}/Variables.sh
source ${SCRIPT_PATH}/Utils.sh

ValidateDirExists ${BUILD_DIR_STM32F303XC}

ELF_PATH=${BUILD_DIR_STM32F303XC}/Applications/${APP_NAME}/${APP_NAME}.elf

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
#!/bin/bash

CUR_PATH=$(dirname $(realpath "$0"))
SCRIPT_PATH=${CUR_PATH}/../

source ${SCRIPT_PATH}/Variables.sh
source ${SCRIPT_PATH}/Utils.sh

# TODO: Update the script so that FONT_NAME and SIZE are arrays
OUTPUT_FOLDER=$1
FONT_NAME="$2"
SIZE=$3

GenerateFont()
{
    python3 ${EXTERNAL_PATH}/TTF2BMH-fork/src/ttf2bmh.py \
        --output_folder ${OUTPUT_FOLDER} \
        --ttf_folder ${FONTS_PATH} \
        --variable_width \
        --ascii \
        --struct \
        --font "$1" \
        --fontsize $2

    rm ${OUTPUT_FOLDER}/$(echo "$1" | tr ' ' '_')/*.png
}

GenerateFont "${FONT_NAME}" ${SIZE}
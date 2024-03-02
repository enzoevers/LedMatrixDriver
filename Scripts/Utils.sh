#!/bin/bash

ValidateDirExists() {
    DIR_PATH=$1
    
    if [ -d ${DIR_PATH} ]; then
        echo "Using path ${DIR_PATH}"
    else
        echo "Path '${DIR_PATH}' does not exist"
        exit
    fi
}

ValidateFileExists() {
    FILE_PATH=$1
    
    if [ -f ${FILE_PATH} ]; then
        echo "Using file path ${FILE_PATH}"
    else
        echo "Path '${FILE_PATH}' does not exist"
        exit
    fi
}

CreateBuildDirectoryIfNotExist() {
    BUILD_DIR_PATH=$1

    echo "Looking for ${BUILD_DIR_PATH}"
    if [ ! -d ${BUILD_DIR_PATH} ]; then
        echo "Did not find ${BUILD_DIR_PATH}"
        echo "Creating ${BUILD_DIR_PATH}"
        mkdir ${BUILD_DIR_PATH}
    else
        echo "Found ${BUILD_DIR_PATH}"
    fi
}

DeleteBuildDirectoryIfExist() {
    BUILD_DIR_PATH=$1

    echo "Looking for ${BUILD_DIR_PATH}"
    if [ ! -d ${BUILD_DIR_PATH} ]; then
        echo "Did not find ${BUILD_DIR_PATH}"
    else
        echo "Found ${BUILD_DIR_PATH}"
        echo "Removing ${BUILD_DIR_PATH}"
        rm -r ${BUILD_DIR_PATH}
    fi
}
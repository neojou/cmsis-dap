#!/bin/bash

if [ ! -d ./output ]; then
    mkdir output
fi 

toolchain_path="../../../../toolchain"
toolchain_name="gcc-arm-none-eabi-5_4-2016q2"

make TOOLCHAIN_PATH=$toolchain_path TOOLCHAIN_NAME=$toolchain_name HOST=mac $1


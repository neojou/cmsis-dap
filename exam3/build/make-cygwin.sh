#!/bin/bash



if [ ! -d "output" ]; then
   mkdir output
fi 


toolchain_name="gcc-arm-none-eabi-5_4-2016q2-20160622-win32"

if [ ! -d "../../toolchain/"$toolchain_name ]; then
   unzip "../../toolchain/"$toolchain_name".zip" -d "../../toolchain/"$toolchain_name
fi  

make --include-dir ../template TOOLCHAIN_NAME=$toolchain_name HOST=cygwin $1


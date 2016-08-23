#!/bin/bash

if [ ! -d ./output ]; then
    mkdir output
fi 

make HOST=mac -f Makefile $1


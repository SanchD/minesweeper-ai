#!/bin/sh

# cd drv/lis3mdl-pid
# git apply ../../patches/*
# cd ../../

mkdir .build
cd .build
cmake ..
make
cd ..

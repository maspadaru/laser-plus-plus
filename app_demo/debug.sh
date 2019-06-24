#!/bin/bash

################ Build ##########
mkdir -p ../cmake-build-debug/demoapp-build
cd ../cmake-build-debug/demoapp-build
cmake -DCMAKE_BUILD_TYPE=Debug ../../app_demo/
make demoapp
cd ../../app_demo

############ Run GDB ###############

gdb --tui ../cmake-build-debug/demoapp-build/demoapp

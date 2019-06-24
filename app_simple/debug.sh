#!/bin/bash

################ Build ##########
mkdir -p ../cmake-build-debug/simpleapp-build
cd ../cmake-build-debug/simpleapp-build
cmake -DCMAKE_BUILD_TYPE=Debug ../../app_simple/
make simpleapp
cd ../../app_simple

############ Run GDB ###############

gdb --tui --args ../cmake-build-debug/simpleapp-build/simpleapp samples/program_simple.laser samples/simple.stream samples/out.txt

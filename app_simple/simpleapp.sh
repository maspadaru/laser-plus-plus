#!/bin/bash

################ Build ##########
mkdir -p ../cmake-build-release/simpleapp-build
cd ../cmake-build-release/simpleapp-build
cmake -DCMAKE_BUILD_TYPE=Release ../../app_simple/
make simpleapp
cd ../../app_simple

################ Run ###########
../cmake-build-release/simpleapp-build/simpleapp S samples/program_simple.laser samples/simple.stream output.txt
cat output.txt

../cmake-build-release/simpleapp-build/simpleapp S samples/steam_generator.laser samples/steam_generator.stream output.txt
cat output.txt

../cmake-build-release/simpleapp-build/simpleapp R samples/bicycle.laser samples/bicycle.stream output.txt
cat output.txt

rm output.txt

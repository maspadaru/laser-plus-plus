#!/bin/bash

################ Build ##########
mkdir -p ../cmake-build-release/demoapp-build
cd ../cmake-build-release/demoapp-build
cmake -DCMAKE_BUILD_TYPE=Release ../../app_demo/
make demoapp
cd ../../app_demo

################################################

#!/bin/bash
rm -rf cmake_build
mkdir cmake_build
cd cmake_build
cmake ..
make
cd ..

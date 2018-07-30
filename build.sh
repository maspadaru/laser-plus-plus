#!/bin/bash
rm -rf cmake-build
mkdir cmake-build
cd cmake-build
cmake ..
make
cd ..

#!/bin/bash

../cmake-build-release/simpleapp S samples/program_simple.laser samples/simple.stream output.txt
cat output.txt

../cmake-build-release/simpleapp S samples/steam_generator.laser samples/steam_generator.stream output.txt
cat output.txt

../cmake-build-release/simpleapp R samples/bicycle.laser samples/bicycle.stream output.txt
cat output.txt

rm output.txt

#!/bin/bash

../cmake-build-release/simpleapp s samples/program_simple.laser samples/simple.stream output.txt
cat output.txt

../cmake-build-release/simpleapp s samples/steam_generator.laser samples/steam_generator.stream output.txt
cat output.txt

../cmake-build-release/simpleapp r samples/bicycle.laser samples/bicycle.stream output.txt
cat output.txt

rm output.txt

#!/bin/bash

PROJECT_NAME=LaserPP

build_release () {
    mkdir -p cmake-build-release
    cd cmake-build-release
    cmake -DCMAKE_BUILD_TYPE=Release ..
    make
    cd ..
}

build_debug () {
    mkdir -p cmake-build-debug
    cd cmake-build-debug
    cmake -DCMAKE_BUILD_TYPE=Debug ..
    make
    cd ..
}

clean_up () {
    rm -rf cmake-build-debug
    rm -rf cmake-build-release
}

run_project () {
    cmake-build-debug/$PROJECT_NAME
}

debug_project () {
    gdb cmake-build-debug/$PROJECT_NAME --tui
}

if [ $1 = "br" ]; then
    build_release
fi
if [ $1 = "bd" ]; then
    build_debug
fi
if [ $1 = "c" ]; then
    clean_up
fi
if [ $1 = "r" ]; then
    run_project
fi
if [ $1 = "d" ]; then
    debug_project
fi



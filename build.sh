#!/bin/bash

PROJECT_NAME=LaserPP

build_release () {
    rm -rf cmake-build-release
    mkdir cmake-build-release
    cd cmake-build-release
    cmake -DCMAKE_BUILD_TYPE=Release ..
    make
    cd ..
}

build_debug () {
    rm -rf cmake-build-debug
    mkdir cmake-build-debug
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

if [ $1 = "b" ]; then
    echo b
    build_release
fi
if [ $1 = "d" ]; then
    echo d
    build_debug
fi
if [ $1 = "c" ]; then
    echo c
    clean_up
fi
if [ $1 = "r" ]; then
    echo r
    run_project
fi



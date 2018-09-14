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
    cmake-build-debug/$PROJECT_NAME 2> /dev/null
}

run_project_with_debug () {
    cmake-build-debug/$PROJECT_NAME
}

debug_project () {
    gdb cmake-build-debug/$PROJECT_NAME --tui
}

print_help () {
    echo "Usage: laser [br bd c r rd d h]"
    echo "br: Build Release"
    echo "bd: Build Debug"
    echo "c: Clean project"
    echo "r: Run project "
    echo "rd: Run project and print all errors"
    echo "d: Debug project using GDB"
    echo "h: Print help"

}

if [ $# -eq 0 ]; then 
	print_help
elif [ $1 = "br" ]; then
    build_release
elif [ $1 = "bd" ]; then
    build_debug
elif [ $1 = "c" ]; then
    clean_up
elif [ $1 = "r" ]; then
    run_project
elif [ $1 = "rd" ]; then
    run_project_with_debug
elif [ $1 = "d" ]; then
    debug_project
elif [ $1 = "h" ]; then
    print_help
else
	print_help
fi


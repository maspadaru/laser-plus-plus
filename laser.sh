#!/bin/bash

EXECUTABLE=testapp
TEST_EXECUTABLE=run_test
BENCHAPP_EXECUTABLE=benchapp

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
    cmake -DCMAKE_CXX_FLAGS=-pg -DCMAKE_BUILD_TYPE=Debug ..
    make
    cd ..
}

clean_up () {
    rm -rf cmake-build-debug
    rm -rf cmake-build-release
}

run_profile () {
    build_debug 
    cmake-build-debug/$BENCHAPP_EXECUTABLE
    gprof cmake-build-debug/$BENCHAPP_EXECUTABLE > prof.txt
}

run_benchapp () {
    build_release 
    time cmake-build-release/$BENCHAPP_EXECUTABLE
}

run_project_silent () {
    build_debug 
    cmake-build-debug/$EXECUTABLE 2> /dev/null
}

run_project () {
    build_debug 
    cmake-build-debug/$EXECUTABLE
}

test_project () {
    build_debug 
    cmake-build-debug/$TEST_EXECUTABLE
}

debug_project () {
    build_debug 
    gdb cmake-build-debug/$EXECUTABLE --tui
}

debug_benchapp () {
    build_debug 
    gdb cmake-build-debug/$BENCHAPP_EXECUTABLE --tui
}

print_help () {
    echo "Usage: laser [b c d p rsilent r release h t]"
    echo "b: Build Debug"
    echo "c: Clean project"
    echo "d: Debug project using GDB"
    echo "p: Profile application. Generates prof.txt"
    echo "release: Build Release"
    echo "bench: Run Benchmark App"
    echo "h: Print help"
    echo "r: Run project and print all errors"
    echo "rsilent: Run project without printing any errors"
    echo "t: run all tests"

}

if [ $# -eq 0 ]; then 
	print_help
elif [ $1 = "release" ]; then
    build_release
elif [ $1 = "b" ]; then
    build_debug
elif [ $1 = "bench" ]; then
    run_benchapp
elif [ $1 = "p" ]; then
    run_profile
elif [ $1 = "c" ]; then
    clean_up
elif [ $1 = "rsilent" ]; then
    run_project_silent
elif [ $1 = "r" ]; then
    run_project
elif [ $1 = "d" ]; then
    debug_project
elif [ $1 = "db" ]; then
    debug_benchapp
elif [ $1 = "t" ]; then
    test_project
elif [ $1 = "h" ]; then
    print_help
else
	print_help
fi


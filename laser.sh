#!/bin/bash

TESTAPP_EXECUTABLE=laser/testapp
GTEST_EXECUTABLE=laser/run_gtest

make_testapp() {
    mkdir -p cmake-build-debug/laser
    cd cmake-build-debug/laser
    cmake -DCMAKE_BUILD_TYPE=Debug ../..
    make testapp
    cd ../..
}

make_gtest() {
    mkdir -p cmake-build-debug/laser
    cd cmake-build-debug/laser
    cmake -DCMAKE_BUILD_TYPE=Debug ../..
    make run_gtest
    cd ../..
}


clean_up () {
    rm -rf cmake-build-debug
    rm -rf cmake-build-release
}

run_testapp () {
    make_testapp
    cmake-build-debug/$TESTAPP_EXECUTABLE
}

debug_testapp () {
    make_testapp
    gdb cmake-build-debug/$TESTAPP_EXECUTABLE --tui
}

run_gtest () {
    make_gtest
    cmake-build-debug/$GTEST_EXECUTABLE
}

print_help () {
    echo "Usage: laser [c d h r t]"
    echo "c: Clean project"
    echo "d: Debug Test App using GDB"
    echo "h: Print help"
    echo "r: Run Test App"
    echo "t: run all testcases"
    echo " "
}

if [ $# -eq 0 ]; then 
	print_help 
elif [ $1 = "c" ]; then
    clean_up
elif [ $1 = "d" ]; then
    debug_testapp
elif [ $1 = "h" ]; then
    print_help
elif [ $1 = "r" ]; then
    run_testapp
elif [ $1 = "t" ]; then
    run_gtest
else
	print_help
fi

#!/bin/bash

TESTAPP_EXECUTABLE=testapp
GTEST_EXECUTABLE=run_gtest

build_release() {
    mkdir -p cmake-build-release
    cd cmake-build-release
    cmake -DCMAKE_BUILD_TYPE=Release ..
    make # placeholder, should build L++ library
    cd ..
}

build_testapp() {
    mkdir -p cmake-build-debug
    cd cmake-build-debug
    cmake -DCMAKE_BUILD_TYPE=Debug ..
    #make VERBOSE=1 testapp
    make testapp
    cd ..
}

build_gtest() {
    mkdir -p cmake-build-debug
    cd cmake-build-debug
    cmake -DCMAKE_BUILD_TYPE=Debug ..
    make run_gtest
    cd ..
}

clean_up () {
    rm -rf cmake-build-debug
    rm -rf cmake-build-release
}

run_testapp () {
    build_testapp
    cmake-build-debug/$TESTAPP_EXECUTABLE
}

debug_testapp () {
    build_testapp
    gdb cmake-build-debug/$TESTAPP_EXECUTABLE --tui
}

run_gtest () {
    build_gtest
    cmake-build-debug/$GTEST_EXECUTABLE
}

run_valgrind_testapp () {
    build_testapp
    valgrind cmake-build-debug/$TESTAPP_EXECUTABLE
}

run_valgrind_gtest () {
    build_gtest
    valgrind cmake-build-debug/$GTEST_EXECUTABLE
}

run_valgrind_testapp_v () {
    build_testapp
    valgrind -v cmake-build-debug/$TESTAPP_EXECUTABLE 
}

run_valgrind_gtest_v () {
    build_gtest
    valgrind -v cmake-build-debug/$GTEST_EXECUTABLE
}

print_help () {
    echo "Usage: laser.sh [b c d h r t]"
    echo "b: build Laser++"
    echo "c: Clean project"
    echo "d: Debug Test App using GDB"
    echo "h: Print help"
    echo "r: Run Test App"
    echo "t: Run all testcases"
    echo "vr: Run Valgrind on Test App"
    echo "vt: Run valgrind on testcases"
    echo " "
}

if [ $# -eq 0 ]; then 
	print_help 
elif [ $1 = "b" ]; then
    build_release
elif [ $1 = "br" ]; then
    build_testapp
elif [ $1 = "bt" ]; then
    build_gtest
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
elif [ $1 = "vr" ]; then
    run_valgrind_testapp
elif [ $1 = "vt" ]; then
    run_valgrind_gtest_v
elif [ $1 = "vrv" ]; then
    run_valgrind_testapp_v
elif [ $1 = "vtv" ]; then
    run_valgrind_gtest
else
	print_help
fi

#ln -s cmake-build-debug/compile_commands.json
# cmake-build-debug/run_gtest --gtest_filter=AtomsTest.Atom

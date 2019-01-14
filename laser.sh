#!/bin/bash

EXECUTABLE=testapp
TEST_EXECUTABLE=run_test
BENCHAPP_EXECUTABLE=benchapp

build() {
    mkdir -p cmake-build-release
    cd cmake-build-release
    cmake -DCMAKE_BUILD_TYPE=Release ..
    make
    cd ..
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
    build 
    rm -f prof_flat.txt
    rm -f prof_graph.txt
    rm -f gmon.out
    cmake-build-debug/$BENCHAPP_EXECUTABLE $1 $2 $3 $4
    gprof -a -b -p cmake-build-debug/$BENCHAPP_EXECUTABLE > prof_flat.txt
    gprof -a -b -q cmake-build-debug/$BENCHAPP_EXECUTABLE > prof_graph.txt
}

run_benchapp () {
    build 
    cmake-build-release/$BENCHAPP_EXECUTABLE $1 $2 $3 $4 
}

# only difference between benchapp is that this does not compile first
run_benchscript () {
    cmake-build-release/$BENCHAPP_EXECUTABLE $1 $2 $3 $4 
}

run_project () {
    build 
    cmake-build-debug/$EXECUTABLE
}

test_project () {
    build 
    cmake-build-debug/$TEST_EXECUTABLE
}

debug_project () {
    build 
    gdb cmake-build-debug/$EXECUTABLE --tui
}

debug_benchapp () {
    build 
    gdb --tui --args cmake-build-debug/$BENCHAPP_EXECUTABLE $1 $2 $3 $4 
}

print_help () {
    echo "Usage: laser [b bench c d dbench p r h t]"
    echo "b: Build Laser"
    echo "c: Clean project"
    echo "bench: Run Benchmark App"
    echo "d: Debug project using GDB"
    echo "dbench: Debug benchark application using GDB"
    echo "p: Profile application. Generates prof.txt"
    echo "h: Print help"
    echo "r: Run TestApp project"
    echo "t: run all tests"
    echo " "
    echo " Options [bench dbench p] require additional arguments:"
    echo " test_name end_time_of_stream number_of_facts_per_timepoint window_size"

}

if [ $# -eq 0 ]; then 
	print_help 
elif [ $1 = "b" ]; then
    build
elif [ $1 = "bench" ]; then
    run_benchapp "$2" "$3" "$4" "$5"
elif [ $1 = "benchscript" ]; then
    run_benchscript "$2" "$3" "$4" "$5"
elif [ $1 = "p" ]; then
    run_profile "$2" "$3" "$4" "$5"
elif [ $1 = "c" ]; then
    clean_up
elif [ $1 = "r" ]; then
    run_project
elif [ $1 = "d" ]; then
    debug_project
elif [ $1 = "dbench" ]; then
    debug_benchapp "$2" "$3" "$4" "$5"
elif [ $1 = "t" ]; then
    test_project
elif [ $1 = "h" ]; then
    print_help
else
	print_help
fi


#!/bin/bash

EXECUTABLE=testapp
TEST_EXECUTABLE=run_test
BENCHAPP_EXECUTABLE=benchapp

build_release() {
    mkdir -p cmake-build-release
    cd cmake-build-release
    cmake -DCMAKE_BUILD_TYPE=Release ..
    make
    cd ..
}

build_debug() {
    mkdir -p cmake-build-debug
    cd cmake-build-debug
    cmake -DCMAKE_CXX_FLAGS=-pg -DCMAKE_EXE_LINKER_FLAGS=-pg -DCMAKE_SHARED_LINKER_FLAGS=-pg -DCMAKE_BUILD_TYPE=Debug ..
    make
    cd ..
}

build() {
    #build_release
    build_debug
}

clean_up () {
    rm -rf cmake-build-debug
    rm -rf cmake-build-release
}

run_profile () {
    rm -f prof_flat.txt
    rm -f prof_graph.txt
    rm -f gmon.out
    cmake-build-debug/$BENCHAPP_EXECUTABLE $1 $2 $3 $4 $5 $6
    gprof -a -b -p cmake-build-debug/$BENCHAPP_EXECUTABLE > prof_flat.txt
    gprof -a -b -q cmake-build-debug/$BENCHAPP_EXECUTABLE > prof_graph.txt
}

run_benchapp () {
    cmake-build-release/$BENCHAPP_EXECUTABLE $1 $2 $3 $4 $5 $6
}

run_project () {
    cmake-build-debug/$EXECUTABLE
}

test_project () {
    cmake-build-debug/$TEST_EXECUTABLE
}

debug_project () {
    gdb cmake-build-debug/$EXECUTABLE --tui
}

debug_benchapp () {
    gdb --tui --args cmake-build-debug/$BENCHAPP_EXECUTABLE $1 $2 $3 $4 $5 $6 
}

print_help () {
    echo "Usage: laser [b bench c d p r h t]"
    echo "b: Build Laser"
    echo "c: Clean project"
    echo "bench: Run Benchmark App"
    echo "d: Debug project using GDB"
    echo "p: Profile application. Generates prof.txt"
    echo "h: Print help"
    echo "r: Run TestApp project"
    echo "t: run all tests"
    echo " "
    echo " Options [bench p] require additional arguments:"
    echo " test_name end_time_of_stream number_of_facts_per_timepoint window_size stream_file_path output_file_path"

}

if [ $# -eq 0 ]; then 
	print_help 
elif [ $1 = "b" ]; then
    build
elif [ $1 = "bench" ]; then
    run_benchapp "$2" "$3" "$4" "$5" "$6" "$7"
elif [ $1 = "p" ]; then
    run_profile "$2" "$3" "$4" "$5" "$6" "$7"
elif [ $1 = "c" ]; then
    clean_up
elif [ $1 = "r" ]; then
    run_project
elif [ $1 = "d" ]; then
    debug_project
elif [ $1 = "dbench" ]; then
    debug_benchapp "$2" "$3" "$4" "$5" "$6" "$7"
elif [ $1 = "t" ]; then
    test_project
elif [ $1 = "h" ]; then
    print_help
else
	print_help
fi


#!/bin/bash

TESTAPP_EXECUTABLE=testapp-build/testapp
GTEST_EXECUTABLE=test-build/run_test
BENCHAPP_EXECUTABLE=benchapp-build/benchapp

make_benchapp() {
    mkdir -p cmake-build-release/benchapp-build
    cd cmake-build-release/benchapp-build
    cmake -DCMAKE_BUILD_TYPE=Release ../../app_bench/
    make benchapp
    cd ../..
}

make_testapp() {
    mkdir -p cmake-build-debug/testapp-build
    cd cmake-build-debug/testapp-build
    # !! LLVM does not support the -pg flag  
    # !! So remove those fags if compiling from MacOS with LLVM
    cmake -DCMAKE_CXX_FLAGS=-pg -DCMAKE_EXE_LINKER_FLAGS=-pg -DCMAKE_SHARED_LINKER_FLAGS=-pg -DCMAKE_BUILD_TYPE=Debug ../../app_test
    make testapp
    cd ../..
}

make_test() {
    mkdir -p cmake-build-debug/test-build
    cd cmake-build-debug/test-build
    cmake -DCMAKE_BUILD_TYPE=Debug ../../test
    make run_test
    cd ../..
}


clean_up () {
    rm -rf cmake-build-debug
    rm -rf cmake-build-release
}

run_profile () {
    make_benchapp
    rm -f prof_flat.txt
    rm -f prof_graph.txt
    rm -f gmon.out
    cmake-build-debug/$BENCHAPP_EXECUTABLE $1 $2 $3 $4 $5 $6 $7
    gprof -a -b -p cmake-build-debug/$BENCHAPP_EXECUTABLE > prof_flat.txt
    gprof -a -b -q cmake-build-debug/$BENCHAPP_EXECUTABLE > prof_graph.txt
}

run_benchapp () {
    cmake-build-release/$BENCHAPP_EXECUTABLE $1 $2 $3 $4 $5 $6 $7
}

run_testapp () {
    make_testapp
    cmake-build-debug/$TESTAPP_EXECUTABLE
}

debug_testapp () {
    make_testapp
    gdb cmake-build-debug/$TESTAPP_EXECUTABLE --tui
}

run_test () {
    make_test
    cmake-build-debug/$GTEST_EXECUTABLE
}

#debug_benchapp () {
    #gdb --tui --args cmake-build-debug/$BENCHAPP_EXECUTABLE $1 $2 $3 $4 $5 $6 $7
#}

print_help () {
    echo "Usage: laser [b bench c d p r h t]"
    echo "b: Build Test App"
    echo "bbm: Build Benchmark App"
    echo "bench: Run Benchmark App"
    echo "c: Clean project"
    echo "d: Debug Test App using GDB"
    echo "h: Print help"
    echo "p: Profile Benchmark Application. Generates prof.txt"
    echo "r: Run Test App"
    echo "t: run all testcases"
    echo " "
    echo " Options [bench] and [p] require additional arguments:"
    echo " test_id end_time_of_stream number_of_facts_per_timepoint window_size chase_algorithm=(S/R) stream_file_path output_file_path"

}

if [ $# -eq 0 ]; then 
	print_help 
elif [ $1 = "b" ]; then
    make_testapp
elif [ $1 = "bbm" ]; then
    make_benchapp
elif [ $1 = "bench" ]; then
    run_benchapp "$2" "$3" "$4" "$5" "$6" "$7" "$8"
elif [ $1 = "p" ]; then
    run_profile "$2" "$3" "$4" "$5" "$6" "$7" "$8"
elif [ $1 = "c" ]; then
    clean_up
elif [ $1 = "r" ]; then
    run_testapp
elif [ $1 = "d" ]; then
    debug_testapp
elif [ $1 = "t" ]; then
    run_test
elif [ $1 = "h" ]; then
    print_help
else
	print_help
fi

#elif [ $1 = "dbench" ]; then
    #debug_benchapp "$2" "$3" "$4" "$5" "$6" "$7" "$8"

cmake_minimum_required(VERSION 3.7)

# comment out the following lines to use system's default C/C++ compiler
# set(CMAKE_C_COMPILER "clang")
# set(CMAKE_CXX_COMPILER "clang++")
# set(CMAKE_C_COMPILER "gcc")
# set(CMAKE_CXX_COMPILER "g++")

project(LaserPP VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set( CMAKE_EXPORT_COMPILE_COMMANDS ON )

################################### LASER ###################################
include_directories(Laser/include/)
file(GLOB LASER_SOURCES
        "Laser/src/formula/*.cpp"
        "Laser/src/formula/extended/*.cpp"
        "Laser/src/util/*.cpp"
        "Laser/src/rule/*.cpp"
        "Laser/src/rule/filter/*.cpp"
        "Laser/src/core/*.cpp"
        "Laser/src/example/*.cpp"
        "Laser/src/service/*.cpp"
        )

find_package(Threads)

################################## TESTAPP ##################################
add_executable(testapp Laser/test/main.cpp ${LASER_SOURCES})
target_link_libraries(testapp ${CMAKE_THREAD_LIBS_INIT})

################################### TESTS ###################################

find_package(GTest)
if(GTEST_FOUND)
    file(GLOB TEST_SOURCES
        "Laser/test/src/*.cpp"
        "Laser/test/testcase/*.cpp"
    )
    include_directories(${GTEST_INCLUDE_DIRS})
    add_executable(run_gtest Laser/test/test.cpp ${LASER_SOURCES} ${TEST_SOURCES})
    target_link_libraries(run_gtest ${GTEST_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})
endif(GTEST_FOUND)

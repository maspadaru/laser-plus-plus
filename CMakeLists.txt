cmake_minimum_required(VERSION 3.7)

# detele the following 2 lines to use system's default C/C++ compiler
set(CMAKE_C_COMPILER "clang")
set(CMAKE_CXX_COMPILER "clang++")

project(LaserPP VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set( CMAKE_EXPORT_COMPILE_COMMANDS ON )

################################### LASER ###################################
include_directories(include/)
file(GLOB LASER_SOURCES
        "src/formula/*.cpp"
        "src/formula/extended/*.cpp"
        "src/util/*.cpp"
        "src/program/*.cpp"
        "src/rule/*.cpp"
        )


################################## TESTAPP ##################################
include_directories(test/include/)
file(GLOB TESTAPP_SOURCES
        "test/src/*.cpp"
    )

set(TESTAPP_LASER_SOURCES ${LASER_SOURCES} ${TESTAPP_SOURCES})
add_executable(testapp test/main.cpp ${TESTAPP_LASER_SOURCES})

################################## BENCHAPP ##################################
include_directories(benchapp/include/)
file(GLOB BENCHAPP_SOURCES
        "benchapp/src/*.cpp"
    )

set(BENCHAPP_LASER_SOURCES ${LASER_SOURCES} ${BENCHAPP_SOURCES})
add_executable(benchapp benchapp/main.cpp ${BENCHAPP_LASER_SOURCES})

################################### TESTS ###################################

find_package(GTest)
if(GTEST_FOUND)
    file(GLOB TEST_SOURCES
        "test/src/*.cpp"
        "test/testcase/*.cpp"
    )
    include_directories(${GTEST_INCLUDE_DIRS})
    add_executable(run_test test/test.cpp ${LASER_SOURCES} ${TEST_SOURCES})
    target_link_libraries(run_test ${GTEST_LIBRARIES} pthread)
endif(GTEST_FOUND)



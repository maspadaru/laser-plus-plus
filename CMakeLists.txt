cmake_minimum_required(VERSION 3.7)

# comment out the following 2 lines to use system's default C/C++ compiler
# set(CMAKE_C_COMPILER "clang")
# set(CMAKE_CXX_COMPILER "clang++")

project(LaserPP VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set( CMAKE_EXPORT_COMPILE_COMMANDS ON )

################################### LASER ###################################
include_directories(include/)
file(GLOB LASER_SOURCES
        "src/formula/*.cpp"
        "src/formula/extended/*.cpp"
        "src/util/*.cpp"
        "src/rule/*.cpp"
        "src/rule/filter/*.cpp"
        "src/core/*.cpp"
        "src/example/*.cpp"
        )

find_package(Threads)

################################## TESTAPP ##################################
include_directories(test/include/)
file(GLOB TESTAPP_SOURCES
        "test/src/*.cpp"
    )

set(TESTAPP_LASER_SOURCES ${LASER_SOURCES} ${TESTAPP_SOURCES})
add_executable(testapp test/main.cpp ${TESTAPP_LASER_SOURCES})
target_link_libraries(testapp ${CMAKE_THREAD_LIBS_INIT})

################################## BENCHAPP ##################################
include_directories(app_bench/include/)
file(GLOB BENCHAPP_SOURCES
        "app_bench/src/*.cpp"
    )

set(BENCHAPP_LASER_SOURCES ${LASER_SOURCES} ${BENCHAPP_SOURCES})
add_executable(benchapp app_bench/main.cpp ${BENCHAPP_LASER_SOURCES})
target_link_libraries(benchapp ${CMAKE_THREAD_LIBS_INIT})

################################## SIMPLEAPP ##################################
include_directories(app_simple/include/)
file(GLOB SIMPLEAPP_SOURCES
        "app_simple/src/*.cpp"
    )

set(SIMPLEAPP_LASER_SOURCES ${LASER_SOURCES} ${SIMPLEAPP_SOURCES})
add_executable(simpleapp app_simple/main.cpp ${SIMPLEAPP_LASER_SOURCES})
target_link_libraries(simpleapp ${CMAKE_THREAD_LIBS_INIT})

################################### TESTS ###################################

find_package(GTest)
if(GTEST_FOUND)
    file(GLOB TEST_SOURCES
        "test/src/*.cpp"
        "test/testcase/*.cpp"
    )
    include_directories(${GTEST_INCLUDE_DIRS})
    add_executable(run_test test/test.cpp ${LASER_SOURCES} ${TEST_SOURCES})
    target_link_libraries(run_test ${GTEST_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})
endif(GTEST_FOUND)



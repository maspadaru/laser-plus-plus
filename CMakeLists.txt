cmake_minimum_required(VERSION 3.7)
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
include_directories(testapp/include/)
file(GLOB TESTAPP_SOURCES
        "testapp/src/*.cpp"
    )

set(TESTAPP_LASER_SOURCES ${LASER_SOURCES} ${TESTAPP_SOURCES})
add_executable(testapp testapp/main.cpp ${TESTAPP_LASER_SOURCES})


################################### TESTS ###################################

find_package(GTest)
if(GTEST_FOUND)
    file(GLOB TEST_SOURCES
        "src/formula/*.cpp"
        "src/formula/extended/*.cpp"
        "src/util/*.cpp"
        "src/program/*.cpp"
        "src/rule/*.cpp"
    )
    include_directories(${GTEST_INCLUDE_DIRS})
    add_executable(run_test test/test.cpp ${LASER_SOURCES})
    target_link_libraries(run_test ${GTEST_LIBRARIES} pthread)
endif(GTEST_FOUND)



cmake_minimum_required(VERSION 3.7)
project(LaserPP VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
include_directories(include/)
file(GLOB SOURCES
        "src/formula/*.cpp"
        "src/formula/extended/*.cpp"
        "src/input/*.cpp"
        "src/output/*.cpp"
        "src/program/*.cpp"
        "src/rule/*.cpp"
    )
set(SOURCE_FILES ${SOURCES})
add_executable(${PROJECT_NAME} main.cpp ${SOURCE_FILES})
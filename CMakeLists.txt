cmake_minimum_required(VERSION 3.7)
project(LaserPP VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set( CMAKE_EXPORT_COMPILE_COMMANDS ON )
include_directories(include/)
file(GLOB SOURCES
        "src/formula/*.cpp"
        "src/formula/extended/*.cpp"
        "src/input/*.cpp"
        "src/output/*.cpp"
        "src/program/*.cpp"
        "src/rule/*.cpp"
        )


include_directories(testapp/include/)
file(GLOB TESTAPP_SOURCES
        "testapp/src/*.cpp"
    )

set(SOURCE_FILES ${SOURCES} ${TESTAPP_SOURCES} src/formula/pseudo_formula.cpp include/formula/pseudo_formula.h include/formula/formula_type.h)
add_executable(${PROJECT_NAME} testapp/main.cpp ${SOURCE_FILES})

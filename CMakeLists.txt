cmake_minimum_required(VERSION 3.10)

project(satSolver)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb")
set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O2 -Wall")

set(SOURCES
    src/main.cpp 
    src/DPLLsolver.cpp 
    src/CDCLsolver.cpp
)

set(HEADERS
    include/DPLLsolver.h 
    include/CDCLsolver.h
    include/DimacsParser.h
    include/common.h
)

add_executable(satSolver ${SOURCES} ${HEADERS})

target_include_directories(satSolver PRIVATE include)
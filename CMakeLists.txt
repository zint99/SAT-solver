cmake_minimum_required(VERSION 3.10)

project(satSolver)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb")
set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O2 -Wall")

add_executable(satSolver 
    main.cpp 
    DPLLsolver.cpp 
    DPLLsolver.h 
    common.h 
    DimacsParser.h
)

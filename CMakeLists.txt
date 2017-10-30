cmake_minimum_required (VERSION 2.6)
project(SVProject)

set(CMAKE_CXX_FLAGS "-O3 -W -Wall -pedantic -std=c++11")

add_executable (SVproject neuron.cpp main.cpp)


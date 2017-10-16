cmake_minimum_required (VERSION 2.6)
project(SVProject)

set(SMAKE_CXX_FLAGS "-03 -W -Wall -pedantic -std=c++11")
add_executable (SVproject neuron.cpp main.cpp)
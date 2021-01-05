cmake_minimum_required(VERSION 3.10)
project(SystemC_NullHop)

##### SET COMPILER ENVIRONMENT #####
set(CMAKE_CXX_STANDARD 14)

##### SystemC LIBRARY #####
include_directories(libs/systemc-2.3.3/src)
link_directories(libs/systemc-2.3.3/lib-linux64)

##### INCLUDE SOURCE DIRECTORY #####
add_subdirectory(src)
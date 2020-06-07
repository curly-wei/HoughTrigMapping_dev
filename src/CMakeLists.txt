#  ref:
#  https://github.com/curly-wei/small-program/tree/master/opencv_dll_cmake

cmake_minimum_required(VERSION 3.13)

set(project_name HoughMapping)
project(${project_name})

set(now_version "0.1.1")

add_subdirectory(genHoughMap)
add_subdirectory(test_genHoughMap)
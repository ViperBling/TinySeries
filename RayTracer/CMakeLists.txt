cmake_minimum_required(VERSION 3.20)
project(RayTracer)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(OutputBinary ${CMAKE_CURRENT_SOURCE_DIR}/Binary)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OutputBinary})

add_subdirectory(Source)
add_subdirectory(Tests)
cmake_minimum_required(VERSION 3.20)
project(RayTracer)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(Renderer)

include(CTest)
enable_testing()

add_executable(RayTracer main.cpp)
target_link_libraries(RayTracer Renderer)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

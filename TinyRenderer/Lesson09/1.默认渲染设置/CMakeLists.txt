cmake_minimum_required(VERSION 3.0.0)
project(TinyRenderer VERSION 0.1.0)

include(CheckCXXCompilerFlag)
find_package(PkgConfig REQUIRED)

find_package(OpenMP REQUIRED)

find_package(OpenGL REQUIRED)
include_directories(${OpenGL_INCLUDE_DIR})

find_package(GLUT REQUIRED)
include_directories(${GLUT_INCLUDE_DIR})

pkg_search_module(GLFW REQUIRED glfw3)
include_directories(${GLFW_INCLUDE_DIRS})

find_package(GLM REQUIRED)
find_package(GLEW REQUIRED STATIC)

include_directories(${GLM_INCLUDE_DIR})

set(GLEW_H /usr/local/Cellar/glew/2.2.0_1/include/GL)
include_directories(${GLEW_H})

set(GLEW_LINK /usr/local/Cellar/glew/2.2.0_1/lib/libGLEW.dylib)
link_libraries(${GLEW_LINK})

function(enable_cxx_compiler_flag_if_supported flag)
    string(FIND "${CMAKE_CXX_FLAGS}" "${flag}" flag_already_set)
    if(flag_already_set EQUAL -1)
        check_cxx_compiler_flag("${flag}" flag_supported)
        if(flag_supported)
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${flag}" PARENT_SCOPE)
        endif()
        unset(flag_supported CACHE)
    endif()
endfunction()

enable_cxx_compiler_flag_if_supported("-Wall")
enable_cxx_compiler_flag_if_supported("-Wextra")
enable_cxx_compiler_flag_if_supported("-pedantic")
enable_cxx_compiler_flag_if_supported("-std=c++17")
enable_cxx_compiler_flag_if_supported("-O3")
enable_cxx_compiler_flag_if_supported("-fopenmp")

file(GLOB SOURCES *.h *.cpp)

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} ${GLEW_LIBRARY} ${GLFW_LIBRARY})

if (APPLE)
    target_link_libraries(${PROJECT_NAME} "-framework OpenGL")
    target_link_libraries(${PROJECT_NAME} "-framework GLUT")
endif()

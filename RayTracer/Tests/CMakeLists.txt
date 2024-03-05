include_directories(${PROJECT_SOURCE_DIR}/Source)

add_executable(RayTracerTest RayTracerTest.cpp)
target_link_libraries(RayTracerTest SourceLib OpenMP::OpenMP_CXX)
# target_include_directories(RayTracerTest PUBLIC ${PROJECT_SOURCE_DIR}/Source)
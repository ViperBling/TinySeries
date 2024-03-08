include_directories(${PROJECT_SOURCE_DIR}/Source)

add_executable(RayTracerTest RayTracerTest.cpp)
target_link_libraries(RayTracerTest SourceLib)
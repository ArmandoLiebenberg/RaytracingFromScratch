cmake_minimum_required(VERSION 3.25)
project(RaytracingFromScratch)

set(CMAKE_CXX_STANDARD 17)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

add_executable(RaytracingFromScratch main.cpp renderer_math.cpp renderer_math.h objects.h trace_ray_simple.cpp trace_ray_simple.h trace_path.cpp trace_path.h)
target_link_libraries(RaytracingFromScratch ${SDL2_LIBRARIES})
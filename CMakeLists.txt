cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)

project(rasterizer VERSION 0.0.0 LANGUAGES CXX C)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -O3")

add_executable(
	rasterizer
	source/main.cpp
	source/rasterizer.cpp
	source/mesh.cpp
	source/camera.cpp
	source/object.cpp
)

add_custom_command(
	TARGET rasterizer
	PRE_BUILD
    COMMAND
    	${CMAKE_COMMAND} -E copy_directory
    	${CMAKE_SOURCE_DIR}/samples
    	${CMAKE_CURRENT_BINARY_DIR}/samples
)

# Dependencies
include_directories(SYSTEM
	third_party/linalg
)

include_directories(rasterizer
	include
)

cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)

project(rasterizer VERSION 0.0.0 LANGUAGES CXX C)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)

add_executable(rasterizer
	source/main.cpp
	source/rasterizer.cpp
	source/mesh.cpp
	source/camera.cpp
)

# Dependencies
include_directories(rasterizer
	include
	third_party/linalg
)

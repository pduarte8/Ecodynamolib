cmake_minimum_required(VERSION 2.8)

project(EcoDynamo)
enable_language(C CXX)

set (PROJECT_VERSION_MAJOR 1)
set (PROJECT_VERSION_MINOR 0)
set (PROJECT_VERSION_PATCH 0)

set(LIBRARY_OUTPUT_PATH ~/bin/lib)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")

message ("** Project ${PROJECT_NAME} - Building Binary tree path: ${PROJECT_BINARY_DIR}")

add_subdirectory(DLLs)
add_subdirectory(EcoDynamo-UX)

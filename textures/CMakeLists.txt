# Selects the cmake minimum required version
cmake_minimum_required (VERSION 2.8)
# sets the project name
project (textures)
# setting the right output path for the executable file
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
# include the following subdir. It also executes the CMakeLists file inside the
# specified directory
add_subdirectory(${PROJECT_SOURCE_DIR}/glad/src)
#include the main src subdirectory
add_subdirectory(${PROJECT_SOURCE_DIR}/src)

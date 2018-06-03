# Selects the cmake minimum required version
cmake_minimum_required (VERSION 2.6)
# sets the project name
project (Environment-Test)
# setting the right output path for the executable file
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
# The following has to come before add_subdirectory because the sources need
# the include directories
include_directories(${PROJECT_SOURCE_DIR}/include)
# include the following subdir. It also executes the CMakeLists file inside the
# specified directory
add_subdirectory(${PROJECT_SOURCE_DIR}/src)

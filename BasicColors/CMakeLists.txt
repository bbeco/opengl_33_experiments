cmake_minimum_required(VERSION 2.8)

project(BasicColors)
add_subdirectory(${PROJECT_SOURCE_DIR}/include/glad/src)
add_executable(${PROJECT_NAME} "main.cc")
target_include_directories(${PROJECT_NAME} PRIVATE include)
target_include_directories(${PROJECT_NAME} PRIVATE include/glad/include)
target_link_libraries(${PROJECT_NAME} glad -lglfw -ldl)

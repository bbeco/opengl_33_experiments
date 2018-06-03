include_directories(${PROJECT_SOURCE_DIR}/glad/include)
add_executable(Triangle triangle)
# setting linker parameters
target_link_libraries(Triangle glad -lglfw -ldl)

include_directories(${PROJECT_SOURCE_DIR}/glad/include)
add_executable(uniforms uniforms)
# setting linker parameters
target_link_libraries(uniforms glad -lglfw -ldl)

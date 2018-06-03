include_directories(${PROJECT_SOURCE_DIR}/glad/include)
add_executable(textures textures)
# setting linker parameters
target_link_libraries(textures glad -lglfw -ldl)

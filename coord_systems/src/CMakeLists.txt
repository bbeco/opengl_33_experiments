include_directories(${PROJECT_SOURCE_DIR}/glad/include)
#add include direcotory for glm
include_directories(${PROJECT_SOURCE_DIR}/include)
add_executable(coordinate_systems coord_systems)
# setting linker parameters
target_link_libraries(coordinate_systems glad -lglfw -ldl)

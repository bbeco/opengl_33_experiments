include_directories(${PROJECT_SOURCE_DIR}/glad/include)
#add include direcotory for glm
include_directories(${PROJECT_SOURCE_DIR}/include)
add_executable(camera_navigation camera_nav)
# setting linker parameters
target_link_libraries(camera_navigation glad -lglfw -ldl)

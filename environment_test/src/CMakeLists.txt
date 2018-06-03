add_library(glad glad)
add_executable(Environment-Test env_test)
target_link_libraries(Environment-Test glad -lglfw -ldl)

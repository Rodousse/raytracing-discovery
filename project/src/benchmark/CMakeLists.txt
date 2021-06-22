
add_executable(benchmark main.cpp)
target_link_libraries(benchmark PUBLIC engine benchmark::benchmark)

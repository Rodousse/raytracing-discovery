
add_executable(benchmark main.cpp)
target_link_libraries(benchmark PUBLIC engine OpenMP::OpenMP_CXX benchmark::benchmark)

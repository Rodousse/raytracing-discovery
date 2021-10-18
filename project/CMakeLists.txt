cmake_minimum_required(VERSION 3.12)

project(Raytracer
        VERSION 0.0.1
        DESCRIPTION "Simple raytracer using c++"
        LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


option(BUILD_BENCHMARK "Build benchmark" ON)
############################## GLOBAL DEF ############################

include(GenerateExportHeader)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(FetchContent)
include(FetchImguiTarget)

# Path to use for cmake generated files
set(GENERATED_DIR "${CMAKE_BINARY_DIR}/generated")
message(STATUS "Generated files goes in : ${GENERATED_DIR}")
file(MAKE_DIRECTORY ${GENERATED_DIR})

# Disable all msvc complains about class export
if ("${CMAKE_CXX_COMPILER_ID}" MATCHES "MSVC")
    add_compile_options(/wd4251)
endif()

# Resource path
set(RESOURCE_PATH "${PROJECT_SOURCE_DIR}/resources")
configure_file(cmake/defines.h.in ${GENERATED_DIR}/defines.h @ONLY)
include_directories("${CMAKE_BINARY_DIR}/include")

############################## THIRD_PARTY ###########################

find_package(OpenGL REQUIRED)
find_package(Eigen3 REQUIRED)
find_package(Stbipp REQUIRED)
find_package(Assimp REQUIRED)
find_package(OpenMP REQUIRED)
find_package(GLEW REQUIRED)
find_package(glfw3 REQUIRED)
fetch_imgui_target()


############################## SOURCES ###############################

add_subdirectory(src/engine)
add_subdirectory(src/app)
if(BUILD_BENCHMARK)
    find_package(benchmark REQUIRED)
    add_subdirectory(src/benchmark)
endif()

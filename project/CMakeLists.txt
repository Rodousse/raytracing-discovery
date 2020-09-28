cmake_minimum_required(VERSION 3.12)

project(Raytracer
        VERSION 0.0.1
        DESCRIPTION "Simple raytracer using c++"
        LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


############################## GLOBAL DEF ############################

include(GenerateExportHeader)

# Path to use for cmake generated files
set(GENERATED_DIR "${CMAKE_BINARY_DIR}/generated")
message(STATUS "Generated files goes in : ${GENERATED_DIR}")
file(MAKE_DIRECTORY ${GENERATED_DIR})

# Disable all msvc complains about class export
if ("${CMAKE_CXX_COMPILER_ID}" MATCHES "MSVC")
    add_compile_options(/wd4251)
endif()

# DLL files path to copy next to the executable for windows
set(WIN_SHARED_LIBRARIES "")

# Resource path
set(RESOURCE_PATH "${PROJECT_SOURCE_DIR}/resources")
configure_file(cmake/defines.h.in ${GENERATED_DIR}/defines.h @ONLY)
include_directories("${CMAKE_BINARY_DIR}/include")

############################## THIRD_PARTY ###########################

find_package(Eigen3 REQUIRED)
find_package(Stbipp REQUIRED)
find_package(Assimp REQUIRED)
find_package(OpenMP REQUIRED)

if(${CMAKE_BUILD_TYPE} MATCHES Debug)
    get_target_property(ASSIMP_LIBRARY_PATH assimp::assimp IMPORTED_LOCATION_DEBUG)
    get_target_property(STBIPP_LIBRARY_PATH Stbipp::Stbipp IMPORTED_LOCATION_DEBUG)
else()
    get_target_property(ASSIMP_LIBRARY_PATH assimp::assimp IMPORTED_LOCATION_RELEASE)
    get_target_property(STBIPP_LIBRARY_PATH Stbipp::Stbipp IMPORTED_LOCATION_RELEASE)
endif()

message(${STBIPP_LIBRARY_PATH})
list(APPEND WIN_SHARED_LIBRARIES ${ASSIMP_LIBRARY_PATH})
list(APPEND WIN_SHARED_LIBRARIES ${STBIPP_LIBRARY_PATH})

############################## SOURCES ###############################

add_subdirectory(src/engine)
add_subdirectory(src/app)

cmake_minimum_required(VERSION 3.8)

project(raytracer_dep LANGUAGES C CXX)

option(BUILD_SHARED_LIBS "Build libraries as shared libs" ON)
option(BUILD_ASSIMP "Build the library assimp" ON)
option(BUILD_EIGEN "Build the library eigen" ON)
option(BUILD_STBIPP "Build the library STBIPP" ON)
option(BUILD_GOOGLE_BENCHMARK "Build the library Google benchmark" ON)
option(BUILD_GLFW "Build the library GLFW" ON)
option(BUILD_GLEW "Build the library GLEW" ON)

message(STATUS ${CMAKE_BUILD_TYPE})

include (ExternalProject)

if(BUILD_EIGEN)
    ExternalProject_Add(eigen
        GIT_REPOSITORY "https://gitlab.com/libeigen/eigen.git"
        GIT_TAG "3.4.0"
        BUILD_IN_SOURCE 0
        BUILD_ALWAYS 0
        INSTALL_DIR "${CMAKE_INSTALL_PREFIX}"
        CMAKE_ARGS -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE} -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR> -DBUILD_SHARED_LIBS:BOOL=${BUILD_SHARED_LIBS}
        )
endif()

if(BUILD_ASSIMP)
    ExternalProject_Add(assimp
        GIT_REPOSITORY "https://github.com/assimp/assimp.git"
        GIT_TAG "3e5feb4"
        BUILD_IN_SOURCE 0
        BUILD_ALWAYS 0
        INSTALL_DIR "${CMAKE_INSTALL_PREFIX}"
        CMAKE_ARGS -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE} -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR> -DASSIMP_INSTALL_PDB=FALSE -DBUILD_SHARED_LIBS:BOOL=${BUILD_SHARED_LIBS} -DASSIMP_BUILD_ASSIMP_TOOLS=OFF -DASSIMP_BUILD_TESTS=OFF
        )
endif()

if(BUILD_STBIPP)
    ExternalProject_Add(stbipp
        GIT_REPOSITORY "https://github.com/Rodousse/stbipp.git"
        GIT_TAG "develop"
        BUILD_IN_SOURCE 0
        BUILD_ALWAYS 0
        INSTALL_DIR "${CMAKE_INSTALL_PREFIX}"
        CMAKE_ARGS -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE} -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR> -DBUILD_SHARED_LIBS:BOOL=${BUILD_SHARED_LIBS} -DSTBIPP_BUILD_EXAMPLE=OFF
        )
endif()

if(BUILD_GOOGLE_BENCHMARK)
    ExternalProject_Add(googleBench
        GIT_REPOSITORY "https://github.com/google/benchmark.git"
        GIT_TAG "v1.5.4"
        BUILD_IN_SOURCE 0
        BUILD_ALWAYS 0
        INSTALL_DIR "${CMAKE_INSTALL_PREFIX}"
        CMAKE_ARGS -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE} -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR> -DBUILD_SHARED_LIBS:BOOL=${BUILD_SHARED_LIBS} -DBENCHMARK_ENABLE_TESTING=OFF -DBENCHMARK_ENABLE_GTEST_TESTS=OFF
        )

if(BUILD_GLFW)
    ExternalProject_Add(glfw
        GIT_REPOSITORY "https://github.com/glfw/glfw.git"
        GIT_TAG "3.3.2"
        BUILD_IN_SOURCE 0
        BUILD_ALWAYS 0
        INSTALL_DIR "${CMAKE_INSTALL_PREFIX}"
        CMAKE_ARGS ${CMAKE_GENERIC_OPTIONS} -DGLFW_BUILD_DOCS=OFF -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR> 
        )
endif()

if(BUILD_GLEW)
    ExternalProject_Add(glew
        URL "https://github.com/nigels-com/glew/releases/download/glew-2.2.0/glew-2.2.0.zip"
        SOURCE_SUBDIR "build/cmake"
        BUILD_IN_SOURCE 0
        BUILD_ALWAYS 0
        INSTALL_DIR "${CMAKE_INSTALL_PREFIX}"
        CMAKE_ARGS ${CMAKE_GENERIC_OPTIONS} -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR> -DBUILD_UTILS:BOOL=OFF
        )
endif()
endif()



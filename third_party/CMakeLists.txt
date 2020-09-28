cmake_minimum_required(VERSION 3.8)

project(raytracer_dep LANGUAGES C CXX)

option(BUILD_ASSIMP "Build the library assimp" ON)
option(BUILD_EIGEN "Build the library eigen" ON)
option(BUILD_STBIPP "Build the library STBIPP" ON)

message(STATUS ${CMAKE_BUILD_TYPE})

include (ExternalProject)

if(BUILD_EIGEN)
    ExternalProject_Add(eigen
        GIT_REPOSITORY "https://gitlab.com/libeigen/eigen.git"
        GIT_TAG "3.3.7"
        BUILD_IN_SOURCE 0
        INSTALL_DIR "${CMAKE_INSTALL_PREFIX}"
        CMAKE_ARGS -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE} -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR> -DBUILD_SHARED_LIBS:BOOL=ON
        )
endif()

if(BUILD_ASSIMP)
    ExternalProject_Add(assimp
        GIT_REPOSITORY "https://github.com/assimp/assimp.git"
        GIT_TAG "3e5feb4"
        BUILD_IN_SOURCE 0
        INSTALL_DIR "${CMAKE_INSTALL_PREFIX}"
        CMAKE_ARGS -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE} -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR> -DASSIMP_INSTALL_PDB=FALSE -DBUILD_SHARED_LIBS:BOOL=ON -DASSIMP_BUILD_ASSIMP_TOOLS=OFF -DASSIMP_BUILD_TESTS=OFF
        )
endif()

if(BUILD_STBIPP)
    ExternalProject_Add(stbipp
        GIT_REPOSITORY "https://github.com/Rodousse/stbipp.git"
        GIT_TAG "origin/develop"
        BUILD_IN_SOURCE 0
        INSTALL_DIR "${CMAKE_INSTALL_PREFIX}"
        CMAKE_ARGS -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE} -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR> -DBUILD_SHARED_LIBS:BOOL=ON -DSTBIPP_SHARED=ON -DSTBIPP_BUILD_EXAMPLE=OFF -DSTBIPP_INSTALL=ON
        )

endif()



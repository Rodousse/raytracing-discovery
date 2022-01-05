set(ENGINE_SRC
    engine/rayCaster.cpp
    engine/data/Camera.cpp
    engine/data/DataIO.cpp
    engine/data/Light.cpp
    engine/data/Material.cpp
    engine/data/Mesh.cpp
    engine/data/PerspectiveCamera.cpp
    engine/geometry/intersection.cpp
    engine/geometry/sampling.cpp
    engine/tonemapping/reinhard.cpp)


set(ENGINE_HEADERS
    engine/rayCaster.hpp
    engine/data/Camera.hpp
    engine/data/DataIO.hpp
    engine/data/Light.hpp
    engine/data/Material.hpp
    engine/data/Mesh.hpp
    engine/data/PerspectiveCamera.hpp
    engine/data/Scene.hpp
    engine/geometry/CommonTypes.hpp
    engine/geometry/Constants.hpp
    engine/geometry/intersection.hpp
    engine/geometry/Ray.hpp
    engine/geometry/sampling.hpp
    engine/tonemapping/reinhard.hpp)


add_library(engine ${ENGINE_SRC} ${ENGINE_HEADERS})


generate_export_header(engine
  EXPORT_MACRO_NAME ENGINE_API
  EXPORT_FILE_NAME ${GENERATED_DIR}/EngineSymbols.h
)


target_include_directories(engine
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<BUILD_INTERFACE:${GENERATED_DIR}>)
target_link_libraries(engine PUBLIC Stbipp::Stbipp Eigen3::Eigen PRIVATE OpenMP::OpenMP_CXX assimp::assimp)
target_compile_definitions(engine PUBLIC $<$<NOT:$<BOOL:${BUILD_SHARED_LIBS}>>:ENGINE_STATIC_DEFINE> _USE_MATH_DEFINES _ENABLE_EXTENDED_ALIGNED_STORAGE PRIVATE ENGINE_EXPORT)

target_compile_options(engine PRIVATE
     $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>:
          -Wall -Wextra -Wshadow -Wnon-virtual-dtor -pedantic -Wold-style-cast -Wunused -Woverloaded-virtual>
     $<$<CXX_COMPILER_ID:GNU>:
          -Wduplicated-cond -Wduplicated-branches -Wnull-dereference -Wuseless-cast>
     $<$<CXX_COMPILER_ID:MSVC>:
          /W4 /w14265 /w14640 /w14826 /w14928>)

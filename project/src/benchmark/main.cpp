#include "defines.h"

#include <benchmark/benchmark.h>
#include <engine/data/DataIO.hpp>
#include <engine/data/Scene.hpp>
#include <engine/rayCaster.hpp>
#include <omp.h>

engine::Scene loadBenchScene();
// Define another benchmark
static void BM_Raytracing(benchmark::State& state)
{
    auto scene = loadBenchScene();
    for(auto _: state)
    {
        omp_set_num_threads(state.range(2));
        scene.cameras.front()->setRenderDimensions(state.range(1), state.range(1));
        const auto result = engine::renderScene(scene, 0, state.range(0), 3);
    }
}
// Benchmark : Samples, Dimensions, Depth, nbThreads
BENCHMARK(BM_Raytracing)->ArgsProduct({{8, 16, 32, 64, 128}, {16, 32, 64}, {1, 2, 4, 6, 8}});

BENCHMARK_MAIN();

engine::Scene loadBenchScene()
{
    engine::Scene scene{};

    if(!engine::IO::loadScene(std::string(RESOURCE_PATH) + "/CornellBoxCubeLighten.fbx", scene))
    {
        throw std::runtime_error(std::string(__func__) + ": Could not load the given scene!");
    }
    scene.backgroundColor = Vector3::Ones() * 0.0;
    return scene;
}


#include "defines.h"

#include <benchmark/benchmark.h>
#include <engine/data/DataIO.hpp>
#include <engine/data/Scene.hpp>
#include <engine/rayCaster.hpp>
#include <omp.h>

std::optional<engine::Scene> loadBenchScene();
// Define another benchmark
static void BM_Raytracing(benchmark::State& state)
{
    auto scene = loadBenchScene();
    engine::RenderProperties params;
    params.maxDepth = 3;
    for(auto _: state)
    {
        omp_set_num_threads(state.range(2));
        scene->cameras.front()->setRenderDimensions(state.range(1), state.range(1));
        params.samples = state.range(0);
        const auto result = engine::renderScene(*scene, 0, params);
    }
}
// Benchmark : Samples, Dimensions, nbThreads
BENCHMARK(BM_Raytracing)->ArgsProduct({{8, 32, 128, 256, 512}, {16, 32, 64}, {1, 2, 4, 6, 8}});

BENCHMARK_MAIN();

std::optional<engine::Scene> loadBenchScene()
{
    std::optional<engine::Scene> scene =
      engine::IO::loadScene(std::string(RESOURCE_PATH) + "/CornellBoxCubeLighten.fbx");
    if(!scene)
    {
        throw std::runtime_error(std::string(__func__) + ": Could not load the given scene!");
    }
    scene->backgroundColor = Vector3::Ones() * 0.0;
    return scene;
}


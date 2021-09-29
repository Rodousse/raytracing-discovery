#include <defines.h>
#include <engine/data/DataIO.hpp>
#include <engine/data/PerspectiveCamera.hpp>
#include <engine/data/Scene.hpp>
#include <engine/rayCaster.hpp>
#include <engine/tonemapping/reinhard.hpp>
#include <iostream>
#include <stbipp/ImageExporter.hpp>
#include <stbipp/ImageImporter.hpp>

int main(int argc, char* argv[])
{
    engine::Scene scene{};

    if(!engine::IO::loadScene(std::string(RESOURCE_PATH) + "/CornellBoxCubeLighten.fbx", scene))
    {
        throw std::runtime_error(std::string(__func__) + ": Could not load the given scene!");
    }
    std::cout << "Scene loaded successfully" << std::endl;

    scene.cameras.front()->setRenderDimensions(1024, 1024);
    scene.cameras.front()->setRenderDimensions(512, 512);
    scene.backgroundColor = Vector3::Ones() * 0.0;
    const int maxDepth = 3;
    const int samples = 512;
    auto result = engine::renderScene(scene, 0, samples, maxDepth);
    if(!stbipp::saveImage("result.jpg", result, stbipp::ImageSaveFormat::RGB))
    {
        std::cerr << "Could not save the result render\n";
        return 1;
    }
    tone::reinhard(result, 0.18);

    if(!stbipp::saveImage("resultToneMapped.jpg", result, stbipp::ImageSaveFormat::RGB))
    {
        std::cerr << "Could not save the result render\n";
        return 1;
    }

    return 0;
}

#include <defines.h>
#include <engine/data/DataIO.hpp>
#include <engine/data/PerspectiveCamera.hpp>
#include <engine/data/Scene.hpp>
#include <engine/rayCaster.hpp>
#include <iostream>
#include <stbipp/ImageExporter.hpp>
#include <stbipp/ImageImporter.hpp>

int main(int argc, char* argv[])
{
    engine::Scene scene{};

    // if(engine::IO::loadScene(std::string(RESOURCE_PATH) + "/CornellBoxSphere.fbx", scene))
    //    if(engine::IO::loadScene(std::string(RESOURCE_PATH) + "/untitled2.fbx", scene))
    if(!engine::IO::loadScene(std::string(RESOURCE_PATH) + "/CornellBoxSphereLighten.fbx", scene))
    {
        throw std::runtime_error(std::string(__func__) + ": Could not load the given scene!");
    }
    std::cout << "Scene loaded successfully" << std::endl;

    scene.cameras.front()->setRenderDimensions(1024, 1024);
    scene.backgroundColor = Vector3::Ones() * 0.0;
    const int maxDepth = 3;
    const int samples = 1024;
    const auto result = engine::renderScene(scene, 0, samples, maxDepth);

    if(!stbipp::saveImage("result.jpg", result, stbipp::ImageSaveFormat::RGB))
    {
        std::cerr << "Could not save the result render\n";
        return 1;
    }

    return 0;
}

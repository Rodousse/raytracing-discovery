#pragma once
#include "EngineSymbols.h"
#include "engine/data/Camera.hpp"
#include "engine/data/Light.hpp"
#include "engine/data/Mesh.hpp"

#include <stbipp/Color.hpp>
#include <vector>

namespace engine
{
struct ENGINE_API Scene
{
    std::vector<std::shared_ptr<Camera>> cameras{};
    std::vector<std::shared_ptr<Material>> materials{};

    std::vector<Mesh> meshes{};
    std::vector<Mesh> emissiveMeshes{};

    Vector3 backgroundColor{0.0, 0.0, 0.0};
};

} // namespace engine

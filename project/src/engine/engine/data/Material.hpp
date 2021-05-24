#pragma once

#include "engine/geometry/CommonTypes.hpp"

namespace engine
{
struct Material
{
    Material() = default;
    Material(const Material&) = default;
    Material(Material&&) = default;
    Material& operator=(const Material&) = default;
    Material& operator=(Material&&) = default;
    virtual ~Material() = default;

    Vector3 diffuseColor{1.0, 1.0, 1.0};
};

struct EmissiveMaterial : Material
{
    Vector3 emissiveColor{1.0, 1.0, 1.0};
};

} // namespace engine

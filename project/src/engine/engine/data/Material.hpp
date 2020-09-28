#pragma once

#include "engine/geometry/CommonTypes.hpp"

namespace engine
{
struct Material
{
    Vector3 diffuseColor{1.0, 1.0, 1.0};
    virtual ~Material() = default;
};

struct EmissiveMaterial : Material
{
    Vector3 emissiveColor{1.0, 1.0, 1.0};
};

} // namespace engine

#pragma once

#include "engine/data/Mesh.hpp"
#include "engine/geometry/CommonTypes.hpp"

namespace engine
{
struct EmissiveMesh : public Mesh
{
    Vector3 emissiveColor{1.0, 1.0, 1.0};
};

} // namespace engine

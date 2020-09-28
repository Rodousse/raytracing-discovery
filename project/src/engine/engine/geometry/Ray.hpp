#pragma once
#include "EngineSymbols.h"
#include "engine/data/Mesh.hpp"
#include "engine/geometry/CommonTypes.hpp"

namespace engine
{
struct ENGINE_API Ray
{
    Vector3 origin;
    Vector3 dir;
};

} // namespace engine

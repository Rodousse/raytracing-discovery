#include "engine/data/Mesh.hpp"

namespace engine
{
void Mesh::refreshBoundingBox()
{
    for(const auto& vertex: vertices)
    {
        Vector3 min = aabb.min.cwiseMin(vertex.pos).eval();
        Vector3 max = aabb.max.cwiseMax(vertex.pos).eval();
        aabb.min = min;
        aabb.max = max;
    }
}

} // namespace engine

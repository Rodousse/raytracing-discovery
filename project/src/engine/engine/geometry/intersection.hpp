#pragma once

#include "EngineSymbols.h"
#include "engine/data/Mesh.hpp"
#include "engine/data/Scene.hpp"
#include "engine/geometry/Ray.hpp"

#include <map>

namespace engine
{
struct ENGINE_API RayTriangleIntersection
{
    bool hit{false};
    Vector3 position{0.0, 0.0, 0.0};
    Vector2 uv{0.0, 0.0};
};

struct ENGINE_API RayMeshIntersection
{
    FaceIndex faceIdx{0};
    const Mesh* mesh{nullptr};
    RayTriangleIntersection faceIntersection{};
};

using OrderedRayMeshIntersections = std::map<Floating, RayMeshIntersection>;

struct ENGINE_API RayAABBIntersection
{
    bool hit{false};
    Vector3 position{0.0, 0.0, 0.0};
};

namespace geometry
{
/**
 * @brief Know if ray intersects the given face, using the Moller-Trumbore algorithm :
 * https://en.wikipedia.org/wiki/M%C3%B6ller%E2%80%93Trumbore_intersection_algorithm
 * @param[in] r Ray to test
 * @param[in] mesh Mesh where the face belongs
 * @param[in] faceIdx Index of the candidate face
 * @param[out] intersectedPoint The position of the intersection if applicable
 * @return True if they intersect
 */
ENGINE_API RayTriangleIntersection isRayIntersectingFace(const Ray& r, const Mesh& mesh, const FaceIndex faceIdx);

/**
 * @brief Know if ray intersects the given axis aligned bounding box, using the Fast Ray-Box Intersection algorithm :
 * https://github.com/erich666/GraphicsGems/blob/master/gems/RayBox.c
 * @param[in] r Ray to test
 * @param[in] aabb Bounding box to test
 * @param[out] intersectedPoint The position of the intersection if applicable
 * @return True if they intersect
 */
ENGINE_API RayAABBIntersection isRayIntersectingBoundingBox(const Ray& r, const AABoundingBox& aabb);

ENGINE_API OrderedRayMeshIntersections findRayLightsIntersections(const Ray& r, const Scene& scene);

ENGINE_API OrderedRayMeshIntersections findRayMeshIntersections(const Ray& r, const Mesh& mesh);

ENGINE_API OrderedRayMeshIntersections findRaySceneIntersections(const Ray& r, const Scene& scene);

} // namespace geometry
} // namespace engine

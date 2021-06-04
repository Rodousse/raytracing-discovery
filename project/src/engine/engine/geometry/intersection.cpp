#include "engine/geometry/intersection.hpp"

#include "engine/geometry/Constants.hpp"

#include <limits>

namespace engine
{
namespace geometry
{
RayTriangleIntersection isRayIntersectingFace(const Ray& r, const Mesh& mesh, const FaceIndex faceIdx)
{
    RayTriangleIntersection returnVal{};
    const auto& face = mesh.faces[faceIdx];
    const auto& vertex0 = mesh.vertices[face.indices[0]].pos;
    const auto& vertex1 = mesh.vertices[face.indices[1]].pos;
    const auto& vertex2 = mesh.vertices[face.indices[2]].pos;
    Vector3 edge1, edge2, h, s, q;
    Floating a, f, u, v;
    edge1 = vertex1 - vertex0;
    edge2 = vertex2 - vertex0;
    h = r.dir.cross(edge2);
    a = edge1.dot(h);
    if(a > -EPSILON && a < EPSILON)
        return returnVal;
    f = static_cast<Floating>(1.0) / a;
    s = r.origin - vertex0;
    u = f * s.dot(h);
    if(u < static_cast<Floating>(0.0) || u > static_cast<Floating>(1.0))
        return returnVal;
    q = s.cross(edge1);
    v = f * r.dir.dot(q);
    if(v < static_cast<Floating>(0.0) || u + v > static_cast<Floating>(1.0))
        return returnVal;
    float t = f * edge2.dot(q);
    if(t > EPSILON)
    {
        returnVal.position = r.origin + r.dir * t;
        returnVal.uv = {u, v};
        returnVal.hit = true;
    }
    return returnVal;
}

RayAABBIntersection isRayIntersectingBoundingBox(const Ray& r, const AABoundingBox& aabb)
{
    RayAABBIntersection returnVal{};
    bool inside = true;
    enum class Quadrant
    {
        Left,
        Right,
        Middle
    };

    std::array<Quadrant, 3> quadrant;
    std::size_t i;
    std::size_t whichPlane;
    std::array<Floating, 3> maxT;
    std::array<Floating, 3> candidatePlane;

    /* Find candidate planes; this loop can be avoided if
    rays cast all from the eye(assume perpsective view) */
    for(i = 0; i < 3; i++)
    {
        if(r.origin[i] < aabb.min[i])
        {
            quadrant[i] = Quadrant::Left;
            candidatePlane[i] = aabb.min[i];
            inside = false;
        }
        else if(r.origin[i] > aabb.max[i])
        {
            quadrant[i] = Quadrant::Right;
            candidatePlane[i] = aabb.max[i];
            inside = false;
        }
        else
        {
            quadrant[i] = Quadrant::Middle;
        }
    }

    /* Ray origin inside bounding box */
    if(inside)
    {
        returnVal.position = r.origin;
        returnVal.hit = true;
        return returnVal;
    }

    /* Calculate T distances to candidate planes */
    for(i = 0; i < 3; i++)
    {
        if(quadrant[i] != Quadrant::Middle && r.dir[i] != 0.)
            maxT[i] = (candidatePlane[i] - r.origin[i]) / r.dir[i];
        else
            maxT[i] = -1.;
    }

    /* Get largest of the maxT's for final choice of intersection */
    whichPlane = 0;
    for(i = 1; i < 3; i++)
    {
        if(maxT[whichPlane] < maxT[i])
        {
            whichPlane = i;
        }
    }

    /* Check final candidate actually inside box */
    if(maxT[whichPlane] < 0.)
    {
        // returnVal.hit = false;
        return returnVal;
    }
    for(i = 0; i < 3; i++)
    {
        if(whichPlane != i)
        {
            returnVal.position[i] = r.origin[i] + maxT[whichPlane] * r.dir[i];
            if(returnVal.position[i] < aabb.min[i] || returnVal.position[i] > aabb.max[i])
            {
                // returnVal.hit = false;
                return returnVal;
            }
        }
        else
        {
            returnVal.position[i] = candidatePlane[i];
        }
    }
    returnVal.hit = true;
    return returnVal; /* ray hits box */
}

OrderedRayMeshIntersections findRayLightsIntersections(const Ray& r, const Scene& scene)
{
    std::vector<Mesh*> candidates{};
    for(const auto& mesh: scene.emissiveMeshes)
    {
        const auto intersectAABB = isRayIntersectingBoundingBox(r, mesh->aabb);
        if(intersectAABB.hit)
        {
            candidates.emplace_back(mesh.get());
        }
    }

    OrderedRayMeshIntersections resultVal{};
    for(const auto& candidate: candidates)
    {
        const auto intersection = findRayMeshIntersections(r, *candidate);
        resultVal.insert(intersection.cbegin(), intersection.cend());
    }

    return resultVal;
}

RayMeshIntersection findClosestRayLightsIntersections(const Ray& r, const Scene& scene)
{
    std::vector<Mesh*> candidates{};
    for(const auto& mesh: scene.emissiveMeshes)
    {
        const auto intersectAABB = isRayIntersectingBoundingBox(r, mesh->aabb);
        if(intersectAABB.hit)
        {
            candidates.emplace_back(mesh.get());
        }
    }

    RayMeshIntersection resultVal{};
    Floating closestDist = std::numeric_limits<Floating>::max();
    for(const auto& candidate: candidates)
    {
        auto intersection = findClosestRayMeshIntersections(r, *candidate);
        const Floating dist = (intersection.faceIntersection.position - r.origin).norm();
        if(intersection.faceIntersection.hit && closestDist > dist)
        {
            resultVal = std::move(intersection);
            closestDist = dist;
        }
    }
    return resultVal;
}

OrderedRayMeshIntersections findRaySceneIntersections(const Ray& r, const Scene& scene)
{
    std::vector<Mesh*> candidates{};
    for(const auto& mesh: scene.meshes)
    {
        const auto intersectAABB = isRayIntersectingBoundingBox(r, mesh->aabb);
        if(intersectAABB.hit)
        {
            candidates.emplace_back(mesh.get());
        }
    }

    OrderedRayMeshIntersections resultVal{};
    for(const auto& candidate: candidates)
    {
        const auto intersection = findRayMeshIntersections(r, *candidate);
        resultVal.insert(intersection.cbegin(), intersection.cend());
    }

    return resultVal;
}

RayMeshIntersection findClosestRaySceneIntersections(const Ray& r, const Scene& scene)
{
    std::vector<Mesh*> candidates{};
    for(const auto& mesh: scene.meshes)
    {
        const auto intersectAABB = isRayIntersectingBoundingBox(r, mesh->aabb);
        if(intersectAABB.hit)
        {
            candidates.emplace_back(mesh.get());
        }
    }

    RayMeshIntersection resultVal{};
    Floating closestDist = std::numeric_limits<Floating>::max();
    for(const auto& candidate: candidates)
    {
        auto intersection = findClosestRayMeshIntersections(r, *candidate);
        const Floating dist = (intersection.faceIntersection.position - r.origin).norm();
        if(intersection.faceIntersection.hit && closestDist > dist)
        {
            resultVal = std::move(intersection);
            closestDist = dist;
        }
    }

    return resultVal;
}
OrderedRayMeshIntersections findRayMeshIntersections(const Ray& r, const Mesh& mesh)
{
    OrderedRayMeshIntersections returnVal{};
    for(FaceIndex faceIdx = 0; faceIdx < mesh.faces.size(); ++faceIdx)
    {
        auto faceInter = isRayIntersectingFace(r, mesh, faceIdx);
        if(faceInter.hit)
        {
            const Floating dist = (faceInter.position - r.origin).norm();
            auto& intersection = returnVal.insert({dist, RayMeshIntersection{}}).first->second;
            intersection.faceIntersection = std::move(faceInter);
            intersection.mesh = &mesh;
            intersection.faceIdx = faceIdx;
        }
    }

    return returnVal;
}

RayMeshIntersection findClosestRayMeshIntersections(const Ray& r, const Mesh& mesh)
{
    RayMeshIntersection returnVal{};
    Floating closestDist = std::numeric_limits<Floating>::max();
    for(FaceIndex faceIdx = 0; faceIdx < mesh.faces.size(); ++faceIdx)
    {
        auto faceInter = isRayIntersectingFace(r, mesh, faceIdx);
        if(faceInter.hit)
        {
            const Floating dist = (faceInter.position - r.origin).norm();
            if(closestDist > dist)
            {
                returnVal.faceIntersection = std::move(faceInter);
                returnVal.mesh = &mesh;
                returnVal.faceIdx = faceIdx;
                closestDist = dist;
            }
        }
    }

    return returnVal;
}

} // namespace geometry
} // namespace engine

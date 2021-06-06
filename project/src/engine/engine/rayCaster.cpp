#include "engine/rayCaster.hpp"

#include "engine/geometry/Constants.hpp"
#include "engine/geometry/intersection.hpp"
#include "engine/geometry/sampling.hpp"

#include <exception>
#include <iostream>

namespace engine
{
stbipp::Image renderScene(const Scene& scene, unsigned int cameraIdx, unsigned int samples, unsigned int maxDepth)
{
    if(cameraIdx >= scene.cameras.size())
    {
        throw std::out_of_range(std::string(__func__) + ": Trying to render with camera index out of range");
    }
    return renderScene(scene, scene.cameras[cameraIdx], samples, maxDepth);
}

stbipp::Image renderScene(const Scene& scene,
                          std::shared_ptr<Camera> camera,
                          unsigned int samples,
                          unsigned int maxDepth)
{
    std::vector<Ray> rays{};
    const auto& screenDimensions = camera->renderDimensions();
    stbipp::Image renderTarget(screenDimensions.x(), screenDimensions.y(), stbipp::Color3f(0.0f));

    rays.reserve(camera->widthRenderDimensions() * camera->heightRenderDimensions());
    for(int pixelY = 0; pixelY < screenDimensions[1]; ++pixelY)
    {
        for(int pixelX = 0; pixelX < screenDimensions[0]; ++pixelX)
        {
            rays.emplace_back(camera->generateRay(Eigen::Vector2i{pixelX, pixelY}));
        }
    }
    int pixelY = 0;
    int nbRayTraced = 0;
#pragma omp parallel shared(renderTarget, rays, nbRayTraced) private(pixelY)
#pragma omp for schedule(dynamic, 1)
    for(pixelY = 0; pixelY < screenDimensions[1]; ++pixelY)
    {
        for(int pixelX = 0; pixelX < screenDimensions[0]; ++pixelX)
        {
            for(unsigned int sampleCount = 0; sampleCount < samples; ++sampleCount)
            {
                const auto pixelColor = castRay(rays[pixelX + pixelY * screenDimensions[0]], scene, 0, maxDepth);
                renderTarget(pixelX, pixelY) += stbipp::Color4f(pixelColor.x(), pixelColor.y(), pixelColor.z(), 1.0f);
            }
            renderTarget(pixelX, pixelY) /= stbipp::Color4f(samples);
            renderTarget(pixelX, pixelY) =
              stbipp::Color4f(std::min(std::max(std::sqrt(renderTarget(pixelX, pixelY).r()), 0.0f), 1.0f),
                              std::min(std::max(std::sqrt(renderTarget(pixelX, pixelY).g()), 0.0f), 1.0f),
                              std::min(std::max(std::sqrt(renderTarget(pixelX, pixelY).b()), 0.0f), 1.0f));
            ++nbRayTraced;
            std::cout << "Achieved : " << int(100 * static_cast<double>(nbRayTraced) / rays.size()) << "%" << std::endl;
        }
    }

    return renderTarget;
}

Vector3 castRay(const Ray& r, const Scene& scene, unsigned int depth, unsigned int maxDepth)
{
    if(depth == maxDepth)
    {
        return {0.0, 0.0, 0.0};
    }

    const auto orderedLightIntersections = geometry::findRayLightsIntersections(r, scene);
    const auto orderedMeshIntersections = geometry::findRaySceneIntersections(r, scene);
    if(orderedMeshIntersections.empty() && orderedLightIntersections.empty())
    {
        return scene.backgroundColor;
    }
    const auto nearestLightIntersection = orderedLightIntersections.cbegin();
    if(!orderedLightIntersections.empty() &&
       (orderedMeshIntersections.empty() ||
        (nearestLightIntersection->first < orderedMeshIntersections.cbegin()->first)))
    {
        return dynamic_cast<EmissiveMaterial*>(nearestLightIntersection->second.mesh->material.get())->emissiveColor;
    }
    const auto& nearestIntersection = orderedMeshIntersections.cbegin()->second;
    const auto* hitMesh = nearestIntersection.mesh;
    Ray newRay{};
    Vector3 normal = hitMesh->faces[nearestIntersection.faceIdx].normal;

    const auto& face = hitMesh->faces[nearestIntersection.faceIdx];
    const auto& nv0 = hitMesh->vertices[face.indices[0]].normal;
    const auto& nv1 = hitMesh->vertices[face.indices[1]].normal;
    const auto& nv2 = hitMesh->vertices[face.indices[2]].normal;
    const auto& uv = nearestIntersection.faceIntersection.uv;
    const auto& p0 = hitMesh->vertices[face.indices[0]].pos;
    const auto& p1 = hitMesh->vertices[face.indices[1]].pos;
    const auto& p2 = hitMesh->vertices[face.indices[2]].pos;
    const auto r0 = p0 + (p0 - nearestIntersection.faceIntersection.position).dot(nv0) * nv0;
    const auto r1 = p1 + (p1 - nearestIntersection.faceIntersection.position).dot(nv1) * nv1;
    const auto r2 = p2 + (p2 - nearestIntersection.faceIntersection.position).dot(nv2) * nv2;

    normal = (1.0 - uv.x() - uv.y()) * nv0 + (uv.x() * nv1) + (uv.y() * nv2);
    normal.normalize();

    newRay.dir = sampling::generateRandomUniformRayDir(normal);
    newRay.origin = (1 - uv.x() - uv.y()) * r0 + uv.x() * r1 + uv.y() * r2;
    // newRay.origin = nearestIntersection.faceIntersection.position ;
    return (hitMesh->material->diffuseColor.cwiseProduct(castRay(newRay, scene, depth + 1, maxDepth))) / 2.0f;
}

} // namespace engine

#include "engine/rayCaster.hpp"

#include "engine/geometry/CommonTypes.hpp"
#include "engine/geometry/Constants.hpp"
#include "engine/geometry/intersection.hpp"

#include <exception>
#include <iostream>
#include <omp.h>

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
#pragma omp parallel shared(renderTarget, rays)
    {
        RenderInformations threadRenderInformations{};
        threadRenderInformations.maxDepth = maxDepth;

#pragma omp for schedule(dynamic, 1)
        for(int pixelY = 0; pixelY < screenDimensions[1]; ++pixelY)
        {
            for(int pixelX = 0; pixelX < screenDimensions[0]; ++pixelX)
            {
                auto& renderInfo = threadRenderInformations;
                stbipp::Color4f pixelColor{0.0f};
                for(unsigned int sampleCount = 0; sampleCount < samples; ++sampleCount)
                {
                    renderInfo.depth = 0;
                    renderInfo.isLight = false;
                    const auto sampleColor = castRay(rays[pixelX + pixelY * screenDimensions[0]], scene, renderInfo);
                    pixelColor += stbipp::Color4f(sampleColor.x(), sampleColor.y(), sampleColor.z(), 1.0f);
                }
                pixelColor /= stbipp::Color4f(samples);
                renderTarget(pixelX, pixelY) = pixelColor;
            }
        }
    }

    return renderTarget;
}

Vector3 castRay(const Ray& r, const Scene& scene, RenderInformations& renderParams)
{
    if(renderParams.depth == renderParams.maxDepth)
    {
        return Vector3::Zero();
    }

    const auto lightIntersections = geometry::findClosestRayLightsIntersections(r, scene);
    const auto meshIntersections = geometry::findClosestRaySceneIntersections(r, scene);
    if(!(meshIntersections.faceIntersection.hit || lightIntersections.faceIntersection.hit))
    {
        return scene.backgroundColor;
    }
    const auto rayMeshDist = (meshIntersections.faceIntersection.position - r.origin).norm();
    const auto rayLightDist = (lightIntersections.faceIntersection.position - r.origin).norm();
    if(lightIntersections.faceIntersection.hit &&
       (!meshIntersections.faceIntersection.hit || (rayLightDist < rayMeshDist)))
    {
        renderParams.isLight = true;
        return dynamic_cast<EmissiveMaterial*>(lightIntersections.mesh->material.get())->emissiveColor;
    }
    const auto* hitMesh = meshIntersections.mesh;
    Vector3 normal = hitMesh->faces[meshIntersections.faceIdx].normal;

    Ray newRay{};
    newRay.dir = sampling::generateRandomUniformHemisphereRayDir(normal, renderParams.sampler);
    newRay.origin = meshIntersections.faceIntersection.position + normal * 1e-4;
    renderParams.depth++;
    const auto radiance = castRay(newRay, scene, renderParams);
    return (hitMesh->material->diffuseColor.cwiseProduct(radiance));
}

} // namespace engine

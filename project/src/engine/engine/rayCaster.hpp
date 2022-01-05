#pragma once

#include "EngineSymbols.h"
#include "engine/data/Scene.hpp"
#include "engine/geometry/CommonTypes.hpp"
#include "engine/geometry/sampling.hpp"

#include <stbipp/Image.hpp>

namespace engine
{
struct ENGINE_API RenderInformations
{
    unsigned int depth{0};
    unsigned int maxDepth;
    sampling::UniformSampler sampler;
    bool isLight{false};
};

ENGINE_API [[nodiscard]] stbipp::Image renderScene(const Scene& scene,
                                                   std::shared_ptr<Camera> camera,
                                                   unsigned int samples,
                                                   unsigned int maxDepth);
ENGINE_API [[nodiscard]] stbipp::Image renderScene(const Scene& scene,
                                                   unsigned int cameraIdx,
                                                   unsigned int samples,
                                                   unsigned int maxDepth);
// ENGINE_API Vector3 castRay(const Ray& r, const Scene& scene, unsigned int depth, unsigned int maxDepth, bool&);
ENGINE_API [[nodiscard]] Vector3 castRay(const Ray& r, const Scene& scene, RenderInformations& renderParams);
} // namespace engine

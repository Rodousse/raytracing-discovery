#pragma once

#include "EngineSymbols.h"
#include "engine/data/Scene.hpp"
#include "engine/geometry/CommonTypes.hpp"
#include "engine/geometry/sampling.hpp"

#include <stbipp/Image.hpp>

namespace engine
{
struct ENGINE_API CasterParameters
{
    unsigned int depth{0};
    unsigned int maxDepth;
    sampling::UniformSampler sampler;
    bool isLight{false};
};

struct RenderProperties
{
    unsigned int maxDepth;
    unsigned int samples;
};

ENGINE_API [[nodiscard]] stbipp::Image renderScene(const Scene& scene,
                                                   std::shared_ptr<Camera> camera,
                                                   const RenderProperties& params);

ENGINE_API [[nodiscard]] stbipp::Image renderScene(const Scene& scene,
                                                   unsigned int cameraIdx,
                                                   const RenderProperties& params);

ENGINE_API void renderSceneRect(const std::shared_ptr<Camera>& camera,
                                const Scene& scene,
                                const RenderProperties& params,
                                const Rect& rect,
                                stbipp::Image& target);

ENGINE_API [[nodiscard]] Vector3 castRay(const Ray& r, const Scene& scene, CasterParameters& renderParams);
} // namespace engine

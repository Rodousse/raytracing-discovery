#pragma once

#include "EngineSymbols.h"

#include <engine/data/Scene.hpp>
#include <stbipp/Image.hpp>

namespace engine
{
ENGINE_API stbipp::Image renderScene(const Scene& scene,
                                     std::shared_ptr<Camera> camera,
                                     unsigned int samples,
                                     unsigned int maxDepth);
ENGINE_API stbipp::Image renderScene(const Scene& scene,
                                     unsigned int cameraIdx,
                                     unsigned int samples,
                                     unsigned int maxDepth);
ENGINE_API Vector3 castRay(const Ray& r, const Scene& scene, unsigned int depth, unsigned int maxDepth);
} // namespace engine

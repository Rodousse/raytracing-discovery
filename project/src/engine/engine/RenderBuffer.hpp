#pragma once

#include "engine/data/Camera.hpp"
#include "engine/data/Scene.hpp"
#include "engine/synchronisation/CommandManager.hpp"

#include <shared_mutex>
#include <stbipp/Image.hpp>
#include <thread>
#include <vector>

namespace engine
{
class RenderBuffer
{
  public:
    RenderBuffer(std::shared_ptr<Camera> camera);

    const std::shared_ptr<Camera>& camera() const;

    const stbipp::Image& renderTarget() const;
    stbipp::Image& renderTarget();
    const stbipp::Image& result() const;
    void transferRenderToResult();

    int renderedSamples() const;

    void clean();

    /// shared mutex to read content
    std::shared_mutex m_readMutex;

  private:
    /// Camera associated with the render
    std::shared_ptr<Camera> m_camera;
    /// Currently rendered image
    stbipp::Image m_renderedImage;
    /// Result of rendering
    std::shared_ptr<stbipp::Image> m_result;
    /// Computed samples in the current buffer
    int m_renderedSamples;
};
} // namespace engine

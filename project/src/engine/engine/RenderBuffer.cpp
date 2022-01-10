#include "engine/RenderBuffer.hpp"

namespace engine
{
RenderBuffer::RenderBuffer(std::shared_ptr<Camera> camera):
  m_camera(std::move(camera)),
  m_renderedImage(m_camera->widthRenderDimensions(), m_camera->heightRenderDimensions()),
  m_result(std::make_shared<stbipp::Image>(m_camera->widthRenderDimensions(), m_camera->heightRenderDimensions()))
{
}

const std::shared_ptr<Camera>& RenderBuffer::camera() const
{
    return m_camera;
}

stbipp::Image& RenderBuffer::renderTarget()
{
    return m_renderedImage;
}

const stbipp::Image& RenderBuffer::result() const
{
    return *m_result;
}

int RenderBuffer::renderedSamples() const
{
    return m_renderedSamples;
}

void RenderBuffer::clean()
{
    m_renderedImage.fill(stbipp::Color4f(0.0f));
    m_renderedSamples = 0;
}

void RenderBuffer::transferRenderToResult()
{
    std::unique_lock<std::shared_mutex> lock(m_readMutex);
    *m_result = m_renderedImage;
    m_renderedSamples++;
}

// void RenderBuffer::tonemapResult()
//{
//    std::unique_lock<std::shared_mutex> lock(m_readMutex);
//    *m_result = m_renderedImage;
//    m_renderedSamples++;
//}

} // namespace engine


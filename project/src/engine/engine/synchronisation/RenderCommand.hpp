#pragma once
#include "engine/RenderBuffer.hpp"
#include "engine/geometry/CommonTypes.hpp"
#include "engine/rayCaster.hpp"
#include "engine/synchronisation/Command.hpp"

namespace engine
{
class RenderRectCommand : public Command
{
  public:
    RenderRectCommand(Scene* scene, RenderBuffer* buffer, RenderProperties params, Rect rect):
      m_buffer(buffer),
      m_scene(scene),
      m_params(params),
      m_rect(rect)
    {
    }
    void execute() override
    {
        renderSceneRect(m_buffer->camera(), *m_scene, m_params, m_rect, m_buffer->renderTarget());
    }

  private:
    RenderBuffer* m_buffer;
    Scene* m_scene;
    RenderProperties m_params;
    Rect m_rect;
};

class RenderBufferSyncCommand : public Command
{
  public:
    RenderBufferSyncCommand(RenderBuffer* buffer): m_buffer(buffer) {}
    void execute() override
    {
        m_buffer->transferRenderToResult();
    }

  private:
    RenderBuffer* m_buffer;
};

class RenderBufferCleanCommand : public Command
{
  public:
    RenderBufferCleanCommand(RenderBuffer* buffer): m_buffer(buffer) {}
    void execute() override
    {
        std::unique_lock<std::shared_mutex> lock(m_buffer->m_readMutex);
        m_buffer->clean();
    }

  private:
    RenderBuffer* m_buffer;
};
} // namespace engine


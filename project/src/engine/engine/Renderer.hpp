#pragma once

#include "engine/RenderBuffer.hpp"
#include "engine/data/Camera.hpp"
#include "engine/data/Scene.hpp"
#include "engine/rayCaster.hpp"
#include "engine/synchronisation/CommandManager.hpp"

#include <condition_variable>
#include <shared_mutex>
#include <thread>
#include <vector>

namespace engine
{
class Renderer : protected CommandManager
{
  public:
    using CommandManager::CommandManager;
    Renderer();
    ~Renderer();

    void setScene(const Scene& scene);
    void setRenderRectSize(const Vector2ui size);
    void setRenderProperties(const RenderProperties& params);
    [[nodiscard]] std::shared_ptr<RenderBuffer> createRenderBuffer(std::shared_ptr<Camera> camera);

  protected:
    void threadMission();
    void createJobsFor(std::shared_ptr<RenderBuffer>& buffer);

    using CommandManager::m_commandQueue;

    void clearQueue();

    struct ThreadSync
    {
        std::mutex queueAccess;
        std::condition_variable jobToDo;
        bool isAlive{true};
    } m_threadSync;

    Scene m_scene;
    Vector2ui m_rectSize{64, 64};
    RenderProperties m_renderParams;
    std::vector<std::shared_ptr<RenderBuffer>> m_renders;
    std::vector<std::thread> m_threadPool;
};
} // namespace engine

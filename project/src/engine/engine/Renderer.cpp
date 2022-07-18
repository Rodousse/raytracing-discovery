#include "engine/Renderer.hpp"

#include "engine/synchronisation/RenderCommand.hpp"

#include <mutex>

namespace engine
{
Renderer::Renderer()
{
    for(unsigned int threadIdx = 0; threadIdx < std::thread::hardware_concurrency() - 1; ++threadIdx)
    {
        m_threadPool.emplace_back(&Renderer::threadMission, this);
    }
}
void Renderer::setScene(const Scene& scene)
{
    clearQueue();
    m_scene = scene;
}
void Renderer::setRenderRectSize(const Vector2ui size)
{
    clearQueue();
    m_rectSize = size;
}
void Renderer::setRenderProperties(const RenderProperties& params)
{
    if(m_renderParams.maxDepth == params.maxDepth && m_renderParams.samples == params.samples)
    {
        return;
    }
    m_renderParams = params;
    clearQueue();
    for(auto& renderBuffer: m_renders)
    {
        {
            std::scoped_lock<std::mutex> lock(m_threadSync.queueAccess);
            auto sync = std::make_unique<RenderBufferCleanCommand>(renderBuffer.get());
            push(std::move(sync));
        }
        createJobsFor(renderBuffer);
    }
}
std::shared_ptr<RenderBuffer> Renderer::createRenderBuffer(std::shared_ptr<Camera> camera)
{
    auto render = m_renders.emplace_back(std::make_shared<RenderBuffer>(std::move(camera)));
    createJobsFor(render);
    return render;
}
void Renderer::threadMission()
{
    while(m_threadSync.isAlive)
    {
        CommandPtr nextCommand;
        std::unique_lock<std::mutex> lock(m_threadSync.queueAccess);
        if(m_commandQueue.empty())
        {
            using namespace std::chrono_literals;
            m_threadSync.jobToDo.wait(lock, [this]() { return !(m_commandQueue.empty() && m_threadSync.isAlive); });
            lock.unlock();
            // std::this_thread::sleep_for(1ms);
        }
        else
        {
            nextCommand = pop();
            lock.unlock();
            nextCommand->execute();
        }
    }
}
void Renderer::createJobsFor(std::shared_ptr<RenderBuffer>& buffer)
{
    auto xBatch = buffer->camera()->widthRenderDimensions() / m_rectSize.x();
    auto yBatch = buffer->camera()->heightRenderDimensions() / m_rectSize.y();
    std::unique_lock<std::mutex> lock(m_threadSync.queueAccess);
    for(auto sample = 0u; sample < m_renderParams.samples; ++sample)
    {
        for(auto y = 0u; y < yBatch; ++y)
        {
            for(auto x = 0u; x < xBatch; ++x)
            {
                Rect rect;
                rect.origin = Vector2ui{x * m_rectSize.x(), y * m_rectSize.y()};
                rect.size = m_rectSize;
                auto cmd = std::make_unique<RenderRectCommand>(&m_scene, buffer.get(), m_renderParams, rect);
                push(std::move(cmd));
            }
        }
        auto sync = std::make_unique<RenderBufferSyncCommand>(buffer.get());
        push(std::move(sync));
    }
    lock.unlock();
    m_threadSync.jobToDo.notify_all();
}
void Renderer::clearQueue()
{
    std::scoped_lock<std::mutex> lock(m_threadSync.queueAccess);
    while(!m_commandQueue.empty())
    {
        m_commandQueue.pop();
    }
}
Renderer::~Renderer()
{
    m_threadSync.isAlive = false;
    m_threadSync.jobToDo.notify_all();
    for(auto& thread: m_threadPool)
    {
        thread.join();
    }
}
} // namespace engine

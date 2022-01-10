#include "engine/synchronisation/CommandManager.hpp"

namespace engine
{
CommandPtr CommandManager::pop()
{
    auto cmd = std::exchange(m_commandQueue.front(), nullptr);
    m_commandQueue.pop();
    return cmd;
}

void CommandManager::push(CommandPtr&& command)
{
    m_commandQueue.emplace(std::move(command));
}

bool CommandManager::empty() const
{
    return m_commandQueue.empty();
}

} // namespace engine

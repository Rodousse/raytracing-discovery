#pragma once

#include "engine/synchronisation/Command.hpp"

#include <memory>
#include <queue>

namespace engine
{
using CommandPtr = std::unique_ptr<Command>;

class CommandManager
{
  public:
    CommandManager() = default;
    CommandManager(const CommandManager&) = delete;
    CommandManager& operator=(const CommandManager&) = delete;
    CommandManager(CommandManager&&) noexcept = default;
    CommandManager& operator=(CommandManager&&) noexcept = default;
    ~CommandManager() = default;

    [[nodiscard]] CommandPtr pop();

    void push(CommandPtr&& command);

    bool empty() const;

  protected:
    std::queue<CommandPtr> m_commandQueue;
};
} // namespace engine

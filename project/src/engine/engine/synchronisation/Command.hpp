#pragma once

namespace engine
{
class Command
{
  public:
    Command() = default;
    Command(const Command&) = delete;
    Command& operator=(const Command&) = delete;
    Command(Command&&) noexcept = default;
    Command& operator=(Command&&) noexcept = default;
    virtual ~Command() = default;

    virtual void execute() = 0;
};

} // namespace engine

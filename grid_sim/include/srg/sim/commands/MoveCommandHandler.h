#pragma once

#include "srg/sim/commands/CommandHandler.h"

namespace srg
{
namespace sim
{
namespace commands
{
class MoveCommandHandler : public CommandHandler
{
public:
    explicit MoveCommandHandler(Simulator* simulator);
    ~MoveCommandHandler() override = default;

    bool handle(containers::SimCommand sc) override;
};
} // namespace commands
} // namespace sim
} // namespace srg

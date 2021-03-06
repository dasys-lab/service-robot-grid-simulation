#include "srg/sim/commands/MoveCommandHandler.h"

#include "srg/Simulator.h"

#include <essentials/IDManager.h>
#include <srg/World.h>
#include <srg/world/Direction.h>

namespace srg
{
namespace sim
{
namespace commands
{

MoveCommandHandler::MoveCommandHandler(Simulator* simulator)
        : CommandHandler(simulator)
{
}

bool MoveCommandHandler::handle(containers::SimCommand sc)
{
    switch (sc.action) {
    case containers::Action::GOLEFT:
        simulator->getWorld()->moveObject(sc.senderID, world::Direction::Left);
        return true;
    case containers::Action::GOUP:
        simulator->getWorld()->moveObject(sc.senderID, world::Direction::Up);
        return true;
    case containers::Action::GORIGHT:
        simulator->getWorld()->moveObject(sc.senderID, world::Direction::Right);
        return true;
    case containers::Action::GODOWN:
        simulator->getWorld()->moveObject(sc.senderID, world::Direction::Down);
        return true;
    default:
        return false;
    }
}
} // namespace commands
} // namespace sim
} // namespace srg
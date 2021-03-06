#pragma once

#include "srg/sim/Sensor.h"
#include "srg/sim/containers/CellPerception.h"

#include <srg/world/Coordinate.h>

#include <essentials/SystemConfig.h>
#include <vector>

namespace essentials
{
class SystemConfig;
}

namespace srg
{
class Simulator;
class World;
namespace world {
    class Cell;
}
namespace sim
{
class Cell;
class SimulatedAgent;
class Coordinate;

class Sensor
{
public:
    Sensor(srg::sim::SimulatedAgent* robot);
    std::vector<containers::CellPerception> createPerceptions(srg::Simulator* simulator);

private:
    std::vector<std::shared_ptr<const world::Cell>>collectCells(world::Coordinate start, world::Coordinate end, srg::World* world);

    SimulatedAgent* robot;
    essentials::SystemConfig& sc;
    uint32_t sightLimit;
};
} // namespace sim
} // namespace srg
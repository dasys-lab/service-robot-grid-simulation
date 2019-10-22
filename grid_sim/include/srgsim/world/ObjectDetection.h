#pragma once

#include "srgsim/containers/Perception.h"
#include "Sensor.h"

#include <vector>
#include <SystemConfig.h>

namespace essentials{
    class SystemConfig;
}

namespace srgsim {
    class Cell;
    class Coordinate;
    class ObjectDetection : public Sensor{
    public:
        ObjectDetection(ServiceRobot* robot);
        std::vector<Perception> createPerceptions(World* world);

    private:
        std::vector<const Cell*> collectCells(Coordinate p0, Coordinate p1, World* world);

        essentials::SystemConfig* sc;
        uint32_t sightLimit;


    };
}
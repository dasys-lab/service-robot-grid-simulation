#pragma once

#include "srg/world/Object.h"

namespace srg
{
class World;
namespace world
{
class ServiceRobot : public Object
{
public:
    explicit ServiceRobot(essentials::IdentifierConstPtr id);
    // friend declarations
    friend World;
    friend std::ostream& operator<<(std::ostream& os, const ServiceRobot& obj);

private:
};
} // namespace world
} // namespace srg

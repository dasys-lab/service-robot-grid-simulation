#pragma once

#include "srg/world/Cell.h"
#include "srg/world/Coordinate.h"
#include "srg/world/RoomType.h"

#include <essentials/IdentifierConstPtr.h>

#include <map>

namespace srg
{
class World;
namespace world
{

class Room
{
public:
    Room(std::string name, essentials::IdentifierConstPtr id);
    void addCell(std::shared_ptr<Cell> cell);
    std::shared_ptr<const Cell> getCell(const Coordinate& coord) const;
    const std::map<Coordinate, std::shared_ptr<Cell>>& getCells() const;
    RoomType getType() const;
    essentials::IdentifierConstPtr getID() const;

    // friend declaration
    friend std::ostream& operator<<(std::ostream& os, const Room& obj);
    friend ::srg::World;

private:
    essentials::IdentifierConstPtr id;
    RoomType type;
    std::string name;
    std::map<Coordinate, std::shared_ptr<Cell>> cells;
};
} // namespace world
} // namespace srg

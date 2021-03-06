#include "srg/world/Room.h"

#include "srg/world/Cell.h"

namespace srg
{
namespace world
{
Room::Room(std::string name, essentials::IdentifierConstPtr id)
        : name(name)
        , id(id)
{
}

RoomType Room::getType() const
{
    return this->type;
}

essentials::IdentifierConstPtr Room::getID() const
{
    return this->id;
}

void Room::addCell(std::shared_ptr<Cell> cell)
{
    cell->room = this;

    auto cellEntry = this->cells.find(cell->coordinate);
    if (cellEntry == this->cells.end()) {
        this->cells.emplace(cell->coordinate, cell);
    }
}

std::shared_ptr<const Cell> Room::getCell(const Coordinate& coordinate) const
{
    if (this->cells.find(coordinate) != this->cells.end()) {
        return this->cells.at(coordinate);
    }
    return nullptr;
}

const std::map<Coordinate, std::shared_ptr<Cell>>& Room::getCells() const
{
    return this->cells;
}

std::ostream& operator<<(std::ostream& os, const Room& obj)
{
    os << "[Room] " << obj.name << " ID: " << obj.id << " Type: " << obj.type;
    return os;
}
} // namespace world
} // namespace srg
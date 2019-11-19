#include "srg/World.h"

#include "srg/world/Cell.h"
#include "srg/world/Door.h"
#include "srg/world/Object.h"
#include "srg/world/Room.h"
#include "srg/world/ServiceRobot.h"

#include <SystemConfig.h>
#include <essentials/IDManager.h>

#include <FileSystem.h>
#include <Tmx.h>

#include <iostream>

namespace srg
{
World::World(essentials::IDManager* idManager)
        : World(essentials::SystemConfig::getInstance()->getConfigPath() + "/textures/Department.tmx", idManager)
{
}

World::World(std::string tmxMapFile, essentials::IDManager* idManager)
        : sizeX(0)
        , sizeY(0)
{
    std::cout << "[World] Loading '" << tmxMapFile << "' world file!" << std::endl;
    Tmx::Map* map = new Tmx::Map();
    map->ParseFile(tmxMapFile);
    for (auto layer : map->GetTileLayers()) {
        // create room
        std::string roomName = layer->GetName();
        uint32_t roomIDInt = layer->GetProperties().GetIntProperty("ID");
        world::Room* room = this->addRoom(roomName, idManager->getID(roomIDInt));

        // create cells
        int roomType = 0;
        for (int x = 0; x < layer->GetWidth(); x++) {
            for (int y = 0; y < layer->GetHeight(); y++) {
                if (layer->GetTile(x, y).gid > 0) {
                    roomType = layer->GetTile(x, y).gid - 17;
                    world::Cell* cell = this->addCell(x, y, room);
                }
            }
        }

        if (room) {
            room->type = static_cast<world::RoomType>(roomType);
            std::cout << "[World] Added " << *room << std::endl;
        }
    }
}

World::~World()
{
    for (auto pair : cellGrid) {
        delete pair.second;
    }
    for (auto& object : objects) {
        delete object.second;
    }
    for (auto& room : rooms) {
        delete room.second;
    }
}

world::Room* World::addRoom(std::string name, essentials::IdentifierConstPtr id)
{
    std::lock_guard<std::recursive_mutex> guard(dataMutex);
    world::Room* room = new world::Room(name, id);
    this->rooms.emplace(id, room);
    return room;
}

world::Cell* World::addCell(uint32_t x, uint32_t y, world::Room* room)
{
    std::lock_guard<std::recursive_mutex> guard(dataMutex);
    world::Coordinate cellCoordinate = world::Coordinate(x, y);

    if (this->cellGrid.find(cellCoordinate) != this->cellGrid.end()) {
        return this->cellGrid.at(cellCoordinate);
    }

    world::Cell* cell = new world::Cell(x, y);
    this->cellGrid.emplace(cell->coordinate, cell);

    // Left
    auto it = this->cellGrid.find(world::Coordinate(x - 1, y));
    if (it != this->cellGrid.end()) {
        cell->left = it->second;
        it->second->right = cell;
    }
    // Up
    it = this->cellGrid.find(world::Coordinate(x, y - 1));
    if (it != this->cellGrid.end()) {
        cell->up = it->second;
        it->second->down = cell;
    }
    // Right
    it = this->cellGrid.find(world::Coordinate(x + 1, y));
    if (it != this->cellGrid.end()) {
        cell->right = it->second;
        it->second->left = cell;
    }
    // Down
    it = this->cellGrid.find(world::Coordinate(x, y + 1));
    if (it != this->cellGrid.end()) {
        cell->down = it->second;
        it->second->up = cell;
    }

    if (x + 1 > this->sizeX) {
        this->sizeX = x + 1;
    }
    if (y + 1 > this->sizeY) {
        this->sizeY = y + 1;
    }

    room->addCell(cell);

    return cell;
}

const world::Object* World::getObject(world::ObjectType type) const
{
    for (auto& object : this->objects) {
        if (object.second->getType() == type) {
            return object.second;
        }
    }
    return nullptr;
}

const world::Cell* World::getCell(const world::Coordinate& coordinate) const
{
    std::lock_guard<std::recursive_mutex> guard(dataMutex);
    if (this->cellGrid.find(coordinate) != this->cellGrid.end()) {
        return this->cellGrid.at(coordinate);
    }
    return nullptr;
}

bool World::placeObject(world::Object* object, world::Coordinate coordinate)
{
    std::lock_guard<std::recursive_mutex> guard(dataMutex);
    auto cellIter = this->cellGrid.find(coordinate);
    if (cellIter == this->cellGrid.end()) {
        return false;
    }

    if (!isPlacementAllowed(cellIter->second, object->getType())) {
        return false;
    }

    object->setCell(cellIter->second);
    return true;
}

const world::Object* World::getObject(essentials::IdentifierConstPtr id) const
{
    auto objectEntry = this->objects.find(id);
    if (objectEntry != this->objects.end()) {
        return objectEntry->second;
    } else {
        return nullptr;
    }
}

world::Object* World::editObject(essentials::IdentifierConstPtr id)
{
    auto objectEntry = this->objects.find(id);
    if (objectEntry != this->objects.end()) {
        return objectEntry->second;
    } else {
        return nullptr;
    }
}

const world::ServiceRobot* World::getRobot(essentials::IdentifierConstPtr id) const
{
    auto robotEntry = this->robots.find(id);
    if (robotEntry != this->robots.end()) {
        return robotEntry->second;
    } else {
        return nullptr;
    }
}

world::ServiceRobot* World::editRobot(essentials::IdentifierConstPtr id)
{
    auto robotEntry = this->robots.find(id);
    if (robotEntry != this->robots.end()) {
        return robotEntry->second;
    } else {
        return nullptr;
    }
}

const std::map<world::Coordinate, world::Cell*>& World::getGrid()
{
    return this->cellGrid;
}

uint32_t World::getSizeX() const
{
    return sizeX;
}

uint32_t World::getSizeY() const
{
    return sizeY;
}

world::ServiceRobot* World::spawnRobot(essentials::IdentifierConstPtr id)
{
    std::lock_guard<std::recursive_mutex> guard(dataMutex);
    // create robot
    world::Object* object = this->createOrUpdateObject(id, world::ObjectType::Robot);
    if (object->getCell()) {
        // robot is already placed, maybe it was spawned already...
        return dynamic_cast<world::ServiceRobot*>(object);
    }

    // search for cell with valid spawn coordinates
    srand(time(NULL));
    const world::Cell* cell = nullptr;
    while (!cell || !isPlacementAllowed(cell, world::ObjectType::Robot)) {
        cell = this->getCell(world::Coordinate(5, 5));
        //        cell = this->getCell(Coordinate(rand() % this->sizeX, rand() % this->sizeY));
    }

    // place robot
    if (this->placeObject(object, cell->coordinate)) {
        // only add robot into list, if it was placed correctly
        world::ServiceRobot* robot = dynamic_cast<world::ServiceRobot*>(object);
        this->addRobot(robot);
        return robot;
    } else {
        return nullptr;
    }
}

world::Object* World::createOrUpdateObject(
        essentials::IdentifierConstPtr id, world::ObjectType type, world::ObjectState state, essentials::IdentifierConstPtr robotID)
{
    std::lock_guard<std::recursive_mutex> guard(dataMutex);
    world::Object* object = editObject(id);
    if (!object) {
        switch (type) {
        case world::ObjectType::Robot:
            object = new world::ServiceRobot(id);
            break;
        case world::ObjectType::Door:
            object = new class world::Door(id, state);
            break;
        default:
            object = new world::Object(type, id, state);
        }
        std::cout << "[World] Created " << *object;
        this->objects.emplace(object->getID(), object);
    }

    object->setType(type);
    object->setState(state);
    // dirty hack, I know! :P
    // TODO: Adapt messages from simulator and allow to set object accordingly
    // TODO: Handle information that object is not carried anymore...
    if (state == world::ObjectState::Carried) {
        if (world::ServiceRobot* robot = this->editRobot(robotID)) {
            //            std::cout << "World::createOrUpdateObject(): Robot " << robotID << " carries " << type << std::endl;
            robot->carriedObject = object;
        } else {
            //            std::cout << "World::createOrUpdateObject(): Robot unknown! " << robotID << std::endl;
        }
    }
    return object;
}

bool World::removeObject(world::Object* object)
{
    std::lock_guard<std::recursive_mutex> guard(dataMutex);
    auto entry = this->objects.find(object->getID());
    if (entry == this->objects.end()) {
        return false;
    }
    this->objects.erase(object->getID());
    object->deleteCell();
    return true;
}

void World::moveObject(essentials::IdentifierConstPtr id, world::Direction direction)
{
    std::lock_guard<std::recursive_mutex> guard(dataMutex);
    world::Object* object = editObject(id);
    if (!object) {
        return;
    }
    world::Cell* goalCell = getNeighbourCell(direction, object);
    if (!goalCell) {
        std::cerr << "World::moveObject(): Cell does not exist! " << std::endl;
        return;
    }
    if (!isPlacementAllowed(goalCell, object->getType())) {
        std::cerr << "World::moveObject(): Placement not allowed on " << goalCell->coordinate << " of type " << object->getType() << std::endl;
        return;
    }
    object->editCell()->removeObject(object);
    goalCell->addObject(object);
}

bool World::addRobot(world::ServiceRobot* robot)
{
    auto robotEntry = this->robots.find(robot->getID());
    if (robotEntry == this->robots.end()) {
        this->robots.emplace(robot->getID(), robot);
        return true;
    } else {
        return false;
    }
}

void World::openDoor(essentials::IdentifierConstPtr id)
{
    std::lock_guard<std::recursive_mutex> guard(dataMutex);
    class world::Door* door = dynamic_cast<class world::Door*>(editObject(id));
    if (door) {
        door->setState(world::ObjectState::Open);
    } else {
        std::cout << "World::openDoor(): No suitable door found with ID: " << *id << std::endl;
    }
}

void World::closeDoor(essentials::IdentifierConstPtr id)
{
    std::lock_guard<std::recursive_mutex> guard(dataMutex);
    class world::Door* door = dynamic_cast<class world::Door*>(editObject(id));
    if (door) {
        door->setState(world::ObjectState::Closed);
    } else {
        std::cout << "World::closeDoor(): No suitable door found with ID: " << *id << std::endl;
    }
}

void World::updateCell(world::Coordinate coordinate, std::vector<world::Object*> objects)
{
    auto cellEntry = this->cellGrid.find(coordinate);
    if (cellEntry == this->cellGrid.end()) {
        return;
    }
    cellEntry->second->update(objects);
}

// INTERNAL METHODS

world::Cell* World::getNeighbourCell(const world::Direction& direction, world::Object* object)
{
    switch (direction) {
    case world::Direction::Left:
        return object->getCell()->left;
    case world::Direction::Up:
        return object->getCell()->up;
    case world::Direction::Right:
        return object->getCell()->right;
    case world::Direction::Down:
        return object->getCell()->down;
    default:
        std::cout << "[World] Unknown Direction: " << direction << std::endl;
        return nullptr;
    }
}

bool World::isPlacementAllowed(const world::Cell* cell, world::ObjectType objectType) const
{
    if (cell->getType() == world::RoomType::Wall) {
        return false;
    }

    for (world::Object* object : cell->getObjects()) {
        if (object->getType() == world::ObjectType::Door) {
            if (objectType == world::ObjectType::Robot) {
                return object->getState() == world::ObjectState::Open;
            } else {
                return false;
            }
        }
    }

    return true;
}

std::recursive_mutex& World::getDataMutex() {
    return this->dataMutex;
}

} // namespace srg

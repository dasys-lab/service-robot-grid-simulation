#include "srgsim/Object.h"

#include "srgsim/Cell.h"

namespace srgsim
{

Object::Object(Type type, essentials::IdentifierConstPtr id)
        : type(type), id(id), cell(nullptr)
{
}

Object::~Object()
{
}

void Object::setCell(Cell* cell)
{
    if (this->cell != nullptr) {
        if (this->cell == cell)
            return;
        this->cell->removeObject(this);
    }
    this->cell = cell;
    this->cell->addObject(this);
}

Cell* Object::getCell()
{
    return this->cell;
}

void Object::deleteCell()
{
    if (!this->cell)
        return;
    Cell* tmp = this->cell;
    this->cell = nullptr;
    tmp->removeObject(this);
}

Type Object::getType() const
{
    return type;
}

essentials::IdentifierConstPtr Object::getID()
{
    return essentials::IdentifierConstPtr(this->id);
}
} // namespace srgsim

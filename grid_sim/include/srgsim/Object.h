#pragma once

#include "srgsim/SRGEnums.h"

#include <essentials/IdentifierConstPtr.h>
#include <essentials/Identifier.h>

namespace srgsim
{

class Cell;

class Object
{
public:
    Object(Type type, essentials::IdentifierConstPtr id);
    virtual ~Object();

    essentials::IdentifierConstPtr getID();
    Type getType() const;

    Cell* getCell();
    void setCell(Cell* cell);
    void deleteCell();

private:
    Cell* cell;
    Type type;
    essentials::IdentifierConstPtr id;
};

} // namespace srgsim

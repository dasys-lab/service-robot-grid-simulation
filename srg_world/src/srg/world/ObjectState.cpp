#include "srg/world/ObjectState.h"

#include <iostream>

namespace srg
{
namespace world
{
/**
 * For getting a string representation of an object state.
 * @param os Outputstream
 * @param type State of an object.
 * @return Outputstream
 */
std::ostream& operator<<(std::ostream& os, const ObjectState& state)
{
    switch (state) {
    case ObjectState::Open:
        os << "Open";
        break;
    case ObjectState::Closed:
        os << "Closed";
        break;
    case ObjectState::Undefined:
        os << "Undefined";
        break;
    default:
        os.setstate(std::ios_base::failbit);
    }
    return os;
}
} // namespace world
} // namespace srg
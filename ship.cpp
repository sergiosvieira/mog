#include "ship.h"

Ship::Ship(): Object()
{

}

Ship::Ship
(
    const Coordinates& position,
    const Vector& velocity,
    unsigned int initialTime,
    unsigned int lifeTime,
    const Vector &acceleration
): Object(position, velocity, initialTime, lifeTime, acceleration, ObjectType::Ship)
{

}

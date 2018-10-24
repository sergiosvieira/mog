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
): Object(position, velocity, initialTime, lifeTime, acceleration, ObjectType::OnWater)
{

}

void Ship::setMaxDepth(double value)
{
    this->maxDepth = value;
}

double Ship::getMaxDepth() const
{
    return this->maxDepth;
}

void Ship::setMinDepth(double value)
{
    this->minDepth = value;
}

double Ship::getMinDepth() const
{
    return this->minDepth;
}

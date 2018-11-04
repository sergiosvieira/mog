#include "water.h"

Water::Water(): TaticalMovingObject()
{

}

Water::Water
(
    const Coordinates& position,
    const Vector& velocity,
    unsigned int initialTime,
    unsigned int lifeTime,
    const Vector &acceleration
): TaticalMovingObject(position, velocity, initialTime, lifeTime, acceleration, ObjectCategory::NavalShip)
{

}

void Water::setMaxDepth(double value)
{
    this->maxDepth = value;
}

double Water::getMaxDepth() const
{
    return this->maxDepth;
}

void Water::setMinDepth(double value)
{
    this->minDepth = value;
}

double Water::getMinDepth() const
{
    return this->minDepth;
}

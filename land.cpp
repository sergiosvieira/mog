#include "land.h"

Land::Land(): TaticalMovingObject()
{

}

Land::Land
(
    const Coordinates& position,
    const Vector& velocity,
    unsigned int initialTime,
    unsigned int lifeTime,
    const Vector &acceleration
): TaticalMovingObject(position, velocity, initialTime, lifeTime, acceleration, ObjectCategory::Vehicle)
{

}

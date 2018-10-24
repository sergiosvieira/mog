#include "car.h"

Car::Car(): Object()
{

}

Car::Car
(
    const Coordinates& position,
    const Vector& velocity,
    unsigned int initialTime,
    unsigned int lifeTime,
    const Vector &acceleration
): Object(position, velocity, initialTime, lifeTime, acceleration, ObjectType::Land)
{

}

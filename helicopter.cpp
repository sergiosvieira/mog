#include "helicopter.h"

Helicopter::Helicopter(): Object()
{

}

Helicopter::Helicopter(const Coordinates &position, const Vector &velocity, unsigned int initialTime, unsigned int lifeTime, const Vector &acceleration):
    Object(position, velocity, initialTime, lifeTime, acceleration, ObjectType::Helicopter)
{

}

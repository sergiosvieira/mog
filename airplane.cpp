#include "airplane.h"

AirPlane::AirPlane(): AirObject()
{
}

AirPlane::AirPlane
(
    const Coordinates &position,
    const Vector &velocity,
    unsigned int initialTime,
    unsigned int lifeTime,
    const Vector& acceleration
): AirObject(position, velocity, initialTime, lifeTime, acceleration, ObjectType::AirPlane)
{

}

#include "airplane.h"

AirPlane::AirPlane(): Object()
{
}

AirPlane::AirPlane
(
    const Coordinates &position,
    const Vector &velocity,
    unsigned int initialTime,
    unsigned int lifeTime,
    const Vector& acceleration
): Object(position, velocity, initialTime, lifeTime, acceleration, ObjectType::AirPlane)
{

}

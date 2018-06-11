#include "airplane.h"

AirPlane::AirPlane(): Object()
{
}

AirPlane::AirPlane
(
    const Coordinates &position,
    const Vector &velocity,
    unsigned int initialTime,
    const Vector& acceleration
): Object(position, velocity, initialTime, acceleration, ObjectType::AirPlane)
{

}

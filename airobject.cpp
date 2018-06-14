#include "airobject.h"

AirObject::AirObject(): Object()
{

}

AirObject::AirObject
(
    const Coordinates& position,
    const Vector& velocity,
    unsigned int initialTime,
    unsigned int lifeTime,
    const Vector& acceleration,
    ObjectType type
): Object(position, velocity, initialTime, lifeTime, acceleration, type)
{

}

void AirObject::setMaxAltitude(double value)
{
    this->maxAltitude = value;
}

double AirObject::getMaxAltitude() const
{
    return this->maxAltitude;
}

void AirObject::setMinAltitude(double value)
{
    this->minAltitude = value;
}

double AirObject::getMinAltitude() const
{
    return this->minAltitude;
}


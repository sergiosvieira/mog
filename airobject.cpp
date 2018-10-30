#include "airobject.h"

Air::Air(): Object()
{

}

Air::Air(const Coordinates& position,
         const Vector& velocity,
         unsigned int initialTime,
         unsigned int lifeTime,
         const Vector& acceleration,
         ObjectCategory type): Object(position, velocity, initialTime, lifeTime, acceleration, type)
{

}

void Air::setMaxAltitude(double value)
{
    this->maxAltitude = value;
}

double Air::getMaxAltitude() const
{
    return this->maxAltitude;
}

void Air::setMinAltitude(double value)
{
    this->minAltitude = value;
}

double Air::getMinAltitude() const
{
    return this->minAltitude;
}


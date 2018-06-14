#include "helicopter.h"

Helicopter::Helicopter(): AirObject()
{

}

Helicopter::Helicopter(const Coordinates &position, const Vector &velocity, unsigned int initialTime, unsigned int lifeTime, const Vector &acceleration):
    AirObject(position, velocity, initialTime, lifeTime, acceleration, ObjectType::Helicopter)
{

}

void Helicopter::setRotationAngle(double value)
{
    this->rotationAngle = value;
}

double Helicopter::getRotationAngle() const
{
    return this->rotationAngle;
}

void Helicopter::setRotationStart(unsigned int value)
{
    this->rotationStart = value;
}

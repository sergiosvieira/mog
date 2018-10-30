#include "helicopter.h"

Helicopter::Helicopter(): Air()
{

}

Helicopter::Helicopter(const Coordinates &position, const Vector &velocity, unsigned int initialTime, unsigned int lifeTime, const Vector &acceleration):
    Air(position, velocity, initialTime, lifeTime, acceleration, ObjectCategory::Helicopter)
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

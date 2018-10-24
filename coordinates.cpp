#include "coordinates.h"

Coordinates::Coordinates()
{

}

Coordinates::Coordinates(const double &x, const double &y, const double &z): x(x), y(y), z(z)
{

}

void Coordinates::setX(const double &x)
{
    this->x = x;
}

void Coordinates::setY(const double &y)
{
    this->y = y;
}

void Coordinates::setZ(const double &z)
{
    this->z = z;
}

double Coordinates::getX() const
{
    return this->x;
}

double Coordinates::getY() const
{
    return this->y;
}

double Coordinates::getZ() const
{
    return this->z;
}

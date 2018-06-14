#include "coordinates.h"

Coordinates::Coordinates()
{

}

Coordinates::Coordinates(const double &x, const double &y): x(x), y(y)
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

double Coordinates::getX() const
{
    return this->x;
}

double Coordinates::getY() const
{
    return this->y;
}

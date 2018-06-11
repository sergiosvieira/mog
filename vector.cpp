#include "vector.h"

#include <cmath>

Vector::Vector()
{

}

Vector::Vector(double x, double y)
{
    this->x = x;
    this->y = y;
}

double Vector::getX() const
{
    return this->x;
}

double Vector::getY() const
{
    return this->y;
}

double Vector::length() const
{
    return sqrt(this->x * this->x + this->y * this->y);
}

double Vector::lenghtSqrt() const
{
    return this->x * this->x + this->y * this->y;
}

Vector Vector::normalize() const
{
    double length = this->length();
    if (length > 0.0)
    {
        Vector u(this->x / length, this->y / length);
        return u;
    }
    return Vector();
}

Vector Vector::perpendicularClockwise()
{
    return Vector(this->y, -this->x);
}
Vector Vector::perpendicularCounterClockwise()
{
    return Vector(-this->y, this->x);
}


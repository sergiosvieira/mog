#include "vector.h"

#include <cmath>

Vector::Vector()
{

}

Vector::Vector(double x, double y, double z)
{
    this->x = x;
    this->y = y;
    this->z = z;
}

void Vector::setX(const double &x)
{
    this->x = x;
}

void Vector::setY(const double &y)
{
    this->y = y;
}

void Vector::setZ(const double &z)
{
    this->z = z;
}

double Vector::getX() const
{
    return this->x;
}

double Vector::getY() const
{
    return this->y;
}

double Vector::getZ() const
{
    return this->z;
}

double Vector::length() const
{
    return sqrt(this->x * this->x + this->y * this->y + this->z * this->z);
}

//double Vector::lenghtSqrt() const
//{
//    return this->x * this->x + this->y * this->y;
//}

//Vector Vector::normalize() const
//{
//    double length = this->length();
//    if (length > 0.0)
//    {
//        Vector u(this->x / length, this->y / length);
//        return u;
//    }
//    return Vector();
//}

//Vector Vector::perpendicularClockwise()
//{
//    return Vector(this->y, -this->x);
//}
//Vector Vector::perpendicularCounterClockwise()
//{
//    return Vector(-this->y, this->x);
//}

//double Vector::angle(const Vector &v)
//{
//    double x1 = this->x; double x2 = v.getX();
//    double y1 = this->getY(); double y2 = v.getY();
//    double dot = x1 * x2 + y1 * y2;
//    double det = x1 * y2 - y1 * x2;
//    double angle = atan2(det, dot);
//    return angle;
//}


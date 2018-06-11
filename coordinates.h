#ifndef COORDINATES_H
#define COORDINATES_H

#include "vector.h"

class Coordinates
{
protected:
    double x = 0.0;
    double y = 0.0;
public:
    Coordinates();
    Coordinates(const double& x, const double& y);
    void setX(const double& x);
    void setY(const double& y);
    double getX() const;
    double getY() const;
    Coordinates& operator+(const Vector& v)
    {
        this->setX(this->getX() + v.getX());
        this->setY(this->getY() + v.getY());
        return *this;
    }
};

#endif // COORDINATES_H

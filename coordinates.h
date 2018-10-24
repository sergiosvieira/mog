#ifndef COORDINATES_H
#define COORDINATES_H

#include "vector.h"

class Coordinates
{
protected:
    double x = 0.0;
    double y = 0.0;
    double z = 0.0;
public:
    Coordinates();
    Coordinates(const double& x, const double& y, const double &z);
    void setX(const double& x);
    void setY(const double& y);
    void setZ(const double& z);
    double getX() const;
    double getY() const;
    double getZ() const;
    Coordinates& operator+(const Vector& v)
    {
        this->setX(this->getX() + v.getX());
        this->setY(this->getY() + v.getY());
        this->setZ((this->getZ() + v.getZ()));
        return *this;
    }
    friend Coordinates& operator+=(Coordinates& p, const Vector& v)
    {
        p.x += v.getX();
        p.y += v.getY();
        p.z += v.getZ();
        return p;
    }
};

#endif // COORDINATES_H

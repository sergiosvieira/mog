#ifndef VECTOR_H
#define VECTOR_H


class Vector
{
protected:
    double x = 0, y = 0;
public:
    Vector();
    Vector(double x, double y);
    double getX() const;
    double getY() const;
    double length() const;
    double lenghtSqrt() const;
    Vector normalize() const;
    Vector perpendicularClockwise();
    Vector perpendicularCounterClockwise();
};

#endif // VECTOR_H

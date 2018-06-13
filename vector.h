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
    double angle(const Vector& v);
    Vector operator*(double scalar)
    {
        return Vector(this->getX() * scalar, this->getY() * scalar);
    }
    Vector& operator*=(double scalar)
    {
        this->x *= scalar;
        this->y *= scalar;
        return *this;
    }
    Vector operator+(const Vector& u)
    {
        return Vector(this->x + u.getX(), this->y + u.getY());
    }
    friend Vector& operator+=(Vector& u, const Vector& v)
    {
        u.x += v.getX();
        u.y += v.getY();
        return u;
    }
    Vector operator-(const Vector& u)
    {
        return Vector(this->x - u.getX(), this->y - u.getY());
    }
    friend Vector& operator-=(Vector& u, const Vector& v)
    {
        u.x -= v.getX();
        u.y -= v.getY();
        return u;
    }
};

#endif // VECTOR_H

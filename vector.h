#ifndef VECTOR_H
#define VECTOR_H


class Vector
{
protected:
    double x = 0, y = 0, z = 0;
public:
    Vector();
    Vector(double x, double y, double z);
    void setX(const double& x);
    void setY(const double& y);
    void setZ(const double& z);
    double getX() const;
    double getY() const;
    double getZ() const;
    double length() const;
//    double lenghtSqrt() const;
//    Vector normalize() const;
//    Vector perpendicularClockwise();
//    Vector perpendicularCounterClockwise();
//    double angle(const Vector& v);
    Vector operator*(double scalar)
    {
        return Vector(this->getX() * scalar, this->getY() * scalar, this->getZ() * scalar);
    }
    Vector& operator*=(double scalar)
    {
        this->x *= scalar;
        this->y *= scalar;
        this->z *= scalar;
        return *this;
    }
    Vector operator+(const Vector& u)
    {
        return Vector(this->x + u.getX(), this->y + u.getY(), this->z + u.getZ());
    }
    friend Vector& operator+=(Vector& u, const Vector& v)
    {
        u.x += v.getX();
        u.y += v.getY();
        u.z += v.getZ();
        return u;
    }
    Vector operator-(const Vector& u)
    {
        return Vector(this->x - u.getX(), this->y - u.getY(), this->z - u.getZ());
    }
    friend Vector& operator-=(Vector& u, const Vector& v)
    {
        u.x -= v.getX();
        u.y -= v.getY();
        u.z -= v.getZ();
        return u;
    }
};

#endif // VECTOR_H

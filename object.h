#ifndef OBJECT_H
#define OBJECT_H

#include "vector.h"
#include "coordinates.h"

enum class MovingPattern
{
    Const_Velocity,
    Const_Acceleration,
    Const_Deceleration
};

enum class ObjectType
{
    AirPlane,
    Helicopter,
    Car,
    Ship
};

class Object
{
protected:
    unsigned int id = 0;
    Coordinates position;
    Vector velocity;
    double initialTime = 0.0;
    Vector acceleration;
    ObjectType type;
public:
    Object();
    Object
    (
        const Coordinates& position,
        const Vector& velocity,
        unsigned int initialTime,
        const Vector& acceleration,
        ObjectType type
    );
    Coordinates getPosition() const;
    Vector getVelocity() const;
    ObjectType getType() const;
    Vector getAcceleraton() const;
    unsigned int getID() const;
    void setID(unsigned int id);
};

#endif // OBJECT_H

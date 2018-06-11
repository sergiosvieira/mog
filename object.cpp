#include "object.h"

Object::Object()
{

}

Object::Object
(const Coordinates &position,
    const Vector &velocity,
    unsigned int initialTime,
    const Vector &acceleration,
    ObjectType type
): position(position),
    velocity(velocity),
    initialTime(initialTime),
    acceleration(acceleration),
    type(type)
{

}

Coordinates Object::getPosition() const
{
    return this->position;
}

Vector Object::getVelocity() const
{
    return this->velocity;
}

ObjectType Object::getType() const
{
    return this->type;
}

Vector Object::getAcceleraton() const
{
    return this->acceleration;
}

Vector Object::getDirection() const
{
    return this->direction;
}

unsigned int Object::getID() const
{
    return this->id;
}

void Object::setID(unsigned int id)
{
    this->id = id;
}

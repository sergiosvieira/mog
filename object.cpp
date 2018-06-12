#include "object.h"

Object::Object()
{

}

Object::Object
(const Coordinates &position,
    const Vector &velocity,
    unsigned int initialTime,
    unsigned int lifeTime,
    const Vector &acceleration,
    ObjectType type
): position(position),
    velocity(velocity),
    initialTime(initialTime),
    lifeTime(lifeTime),
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

unsigned int Object::getInitialTime() const
{
    return this->initialTime;
}

unsigned int Object::getLifeTime() const
{
    return this->lifeTime;
}

void Object::setID(unsigned int id)
{
    this->id = id;
}

void Object::addPattern(const MovingPattern &pattern, int instant)
{
    this->patterns[instant] = pattern;
}

MovingPattern Object::getPattern(int instant)
{
    if (this->patterns.count(instant) > 0)
    {
        this->lastPattern = this->patterns[instant];
    }
    return this->lastPattern;
}

void Object::setActivated(bool flag)
{
    this->isActivated_ = flag;
}

bool Object::isActivated() const
{
    return this->isActivated_;
}

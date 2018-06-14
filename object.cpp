#include "object.h"

#include <vector>

Object::Object()
{

}

Object::Object
(const Coordinates &position,
    const Vector &velocity,
    unsigned int initialTime,
    unsigned int endTime,
    const Vector &acceleration,
    ObjectType type
): position(position),
    velocity(velocity),
    initialTime(initialTime),
    endTime(endTime),
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

void Object::setDirection(const Vector& u)
{
    this->direction = u;
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

unsigned int Object::getEndTime() const
{
    return this->endTime;
}

void Object::setID(unsigned int id)
{
    this->id = id;
}

void Object::addPattern(const MovingPattern &pattern, int time)
{
    this->patterns[time] = pattern;
}

MovingPattern Object::getPattern(int time)
{
    if (this->patterns.count(time) > 0)
    {
        this->lastPattern = this->patterns[time];
    }
    return this->lastPattern;
}

std::vector<int> Object::getAllPatternTime()
{
    std::vector<int> result;
    for (auto kv: this->patterns)
    {
        result.push_back(kv.first);
    }
    return result;
}

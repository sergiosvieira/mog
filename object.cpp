#include "object.h"

#include <vector>
#include <string>



Object::Object()
{

}

Object::Object
(const Coordinates &position,
    const Vector &velocity,
    unsigned int initialTime,
    unsigned int endTime,
    const Vector &acceleration,
    ObjectCategory type
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

ObjectCategory Object::getType() const
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
    auto it = this->patterns.lower_bound(time);
    if (it == this->patterns.end())
    {
        if (this->patterns.size() > 0)
            it = --this->patterns.end();
        else return MovingPattern::Const_Velocity;
    }
    return it->second;
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

void Object::setAreaType(GraphicsViewType type)
{
    areaType = type;
}

GraphicsViewType Object::getAreaType()
{
    return areaType;
}

ObjectCategory Object::typeFromString(const std::string &str)
{
    return mapStrType[str];
}

std::string Object::stringFromType(ObjectCategory type)
{
    return mapTypeStr[type];
}

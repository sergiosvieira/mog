#include "taticalmovingobject.h"

#include <vector>
#include <string>



TaticalMovingObject::TaticalMovingObject()
{

}

TaticalMovingObject::TaticalMovingObject
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

Coordinates TaticalMovingObject::getPosition() const
{
    return this->position;
}

Vector TaticalMovingObject::getVelocity() const
{
    return this->velocity;
}

ObjectCategory TaticalMovingObject::getType() const
{
    return this->type;
}

Vector TaticalMovingObject::getAcceleraton() const
{
    return this->acceleration;
}

void TaticalMovingObject::setDirection(const Vector& u)
{
    this->direction = u;
}

Vector TaticalMovingObject::getDirection() const
{
    return this->direction;
}

unsigned int TaticalMovingObject::getID() const
{
    return this->id;
}

unsigned int TaticalMovingObject::getInitialTime() const
{
    return this->initialTime;
}

unsigned int TaticalMovingObject::getEndTime() const
{
    return this->endTime;
}

void TaticalMovingObject::setID(unsigned int id)
{
    this->id = id;
}

void TaticalMovingObject::addPattern(const MovingPattern &pattern, int time)
{
    this->patterns[time] = pattern;
}

MovingPattern TaticalMovingObject::getPattern(int time)
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

std::vector<int> TaticalMovingObject::getAllPatternTime()
{
    std::vector<int> result;
    for (auto kv: this->patterns)
    {
        result.push_back(kv.first);
    }
    return result;
}

void TaticalMovingObject::setAreaType(GraphicsViewType type)
{
    areaType = type;
}

GraphicsViewType TaticalMovingObject::getAreaType()
{
    return areaType;
}

ObjectCategory TaticalMovingObject::typeFromString(const std::string &str)
{
    return mapStrType[str];
}

std::string TaticalMovingObject::stringFromType(ObjectCategory type)
{
    return mapTypeStr[type];
}

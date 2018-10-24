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

void Object::setAreaType(GraphicsViewType type)
{
    areaType = type;
}

GraphicsViewType Object::getAreaType()
{
    return areaType;
}

ObjectType Object::typeFromString(const std::string &str)
{
    if (str == "Helicopter")
    {
        return ObjectType::Helicopter;
    }
    else if (str == "Missile")
    {
        return ObjectType::Missile;
    }
    else if (str == "Cargo Aircraft")
    {
        return ObjectType::Cargo;
    }
    else if (str == "Boing Airplane")
    {
        return ObjectType::Boing;
    }
    else if (str == "Fighter Jet")
    {
        return ObjectType::Fighter;
    }
    else if (str == "Land")
    {
        return ObjectType::Land;
    }
    else if (str == "On water")
    {
        return ObjectType::OnWater;
    }
    else if (str == "Underwater")
    {
        return ObjectType::Underwater;
    }
    return ObjectType::AirPlane;
}

std::string Object::stringFromType(ObjectType type)
{
    std::map<ObjectType, std::string> m =
    {
        {ObjectType::AirPlane, "Airplane"},
        {ObjectType::Helicopter, "Helicopter"},
        {ObjectType::Missile, "Missile"},
        {ObjectType::Cargo, "Cargo Aircraft"},
        {ObjectType::Boing, "Boing Airplane"},
        {ObjectType::Fighter, "Fighter Jet"},
        {ObjectType::Land, "Land"},
        {ObjectType::OnWater, "On water"},
        {ObjectType::Underwater, "Underwater"},
    };
    return m[type];
}

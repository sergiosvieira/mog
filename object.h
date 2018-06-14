#ifndef OBJECT_H
#define OBJECT_H

#include <map>
#include <vector>
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

using MapOfPatternToInstant = std::map<int, MovingPattern>;

class Object
{
protected:
    unsigned int id = 0;
    Coordinates position;
    Vector velocity;
    unsigned int initialTime = 0;
    unsigned int endTime = 0;
    Vector acceleration;
    Vector direction;
    ObjectType type;
    MapOfPatternToInstant patterns;
    MovingPattern lastPattern;
public:
    Object();
    Object
    (
        const Coordinates& position,
        const Vector& velocity,
        unsigned int initialTime,
        unsigned int lifeTime,
        const Vector& acceleration,
        ObjectType type
    );
    Coordinates getPosition() const;
    Vector getVelocity() const;
    ObjectType getType() const;
    Vector getAcceleraton() const;
    void setDirection(const Vector& u);
    Vector getDirection() const;
    unsigned int getID() const;
    unsigned int getInitialTime() const;
    unsigned int getEndTime() const;
    void setID(unsigned int id);
    void addPattern(const MovingPattern& pattern, int time);
    MovingPattern getPattern(int time);
    std::vector<int> getAllPatternTime();
};

#endif // OBJECT_H

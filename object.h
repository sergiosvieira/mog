#ifndef OBJECT_H
#define OBJECT_H

#include <map>
#include <vector>
#include <qpoint.h>
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
    Missile,
    Cargo,
    Boing,
    Fighter,
    Land,
    OnWater,
    Underwater
};


enum class DistributionType
{
    Uniform,
    Poisson,
    Gaussian,
    LeftSkewed,
    RightSkewed
};

enum class DragWidgetType
{
    ShipNK,
    ShipSK,
    AirportNK,
    AirportSK,
    HelicopterCarNK,
    HelicopterCarSK
};

enum class GraphicsViewType
{
    NK,
    SK
};

struct Station
{
    DragWidgetType type;
    QPointF pos;
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
    GraphicsViewType areaType;

public:
    void setAreaType(GraphicsViewType type);
    GraphicsViewType getAreaType();
    static ObjectType typeFromString(const std::string &str);
    static std::string stringFromType(ObjectType type);

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
    void setType(ObjectType type)
    {
        this->type = type;
    }
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

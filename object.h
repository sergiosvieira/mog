#ifndef OBJECT_H
#define OBJECT_H

#include <map>
#include <vector>
#include <qpoint.h>
#include <QString>
#include <vector>
#include <set>
#include "vector.h"
#include "coordinates.h"
#include "objecttype.h"

enum class MovingPattern
{
    Const_Velocity,
    Const_Acceleration,
    Const_Deceleration
};

struct Pattern
{
    static MovingPattern patternFromString(const std::string& str)
    {
        MovingPattern result = MovingPattern::Const_Velocity;
        if (str == "Constant Acceleration")
        {
            result = MovingPattern::Const_Acceleration;
        }
        else if (str == "Constant Deceleration")
        {
            result = MovingPattern::Const_Deceleration;
        }
        return result;
    }

    static std::string stringFromPattern(MovingPattern type)
    {
        if (type == MovingPattern::Const_Acceleration)
        {
            return "Constant Acceleration";
        }
        else if (type == MovingPattern::Const_Deceleration)
        {
            return "Constant Deceleration";
        }
        return "Constant Velocity";
    }
};

static const std::set<ObjectCategory> airObjectsType = {
    ObjectCategory::PassengerAirPlane,
    ObjectCategory::ExportationAirPlane,
    ObjectCategory::Helicopter,
    ObjectCategory::Missile,
    ObjectCategory::Cargo,
    ObjectCategory::Fighter
};

static const std::set<ObjectCategory> landObjectsType = {
    ObjectCategory::Battletank,
    ObjectCategory::Vehicle,
    ObjectCategory::Infantry
};

static const std::set<ObjectCategory> waterObjectsType = {
    ObjectCategory::FishingShip,
    ObjectCategory::NavalShip
};

static const std::set<ObjectCategory> underwaterObjectsType = {
    ObjectCategory::Submarine,
    ObjectCategory::UnderwaterRobot
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

struct PatternRecord
{
    int instant = 0;
    MovingPattern pattern = MovingPattern::Const_Velocity;
};

class Object
{
private:
    QString name;
protected:
    unsigned int id = 0;
    Coordinates position;
    Vector velocity;
    unsigned int initialTime = 0;
    unsigned int endTime = 0;
    Vector acceleration;
    Vector direction;
    ObjectCategory type;
    MapOfPatternToInstant patterns;
    MovingPattern lastPattern;
    GraphicsViewType areaType;
public:
    void setAreaType(GraphicsViewType type);
    GraphicsViewType getAreaType();
    static ObjectCategory typeFromString(const std::string &str);
    static std::string stringFromType(ObjectCategory type);
public:
    Object();
    Object(const Coordinates& position,
        const Vector& velocity,
        unsigned int initialTime,
        unsigned int lifeTime,
        const Vector& acceleration,
        ObjectCategory type);
    Coordinates getPosition() const;
    Vector getVelocity() const;
    ObjectCategory getType() const;
    void setType(ObjectCategory type)
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
    void setName(const QString &name)
    {
        this->name = name;
    }
    QString getName() const
    {
        return this->name;
    }
};

#endif // OBJECT_H

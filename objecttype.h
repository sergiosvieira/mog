#ifndef OBJECTTYPE_H
#define OBJECTTYPE_H

#include "enum.h"

enum class ObjectCategory
{
    Airplane,
    Helicopter,
    Missile,
    Fighter,
    Cargo,
    Boeing,
    Land,
    Naval,
    Underwater
};

struct ObjectInfo
{
    double maxVelocity = 0.0;
    double minVelocity = 0.0;
    double maxAcceleration = 99.0;
    double minAcceleration = 1.0;
};

struct AirInfo: public Enum<ObjectCategory>, public ObjectInfo
{
    double maxAltitude = 0.0;
    double minAltitude = 0.0;
};

struct AirplaneInfo: public AirInfo
{
    AirplaneInfo()
    {
        this->type = ObjectCategory::Airplane;
        this->category = "Air";
        this->value = "Airplane";
        this->maxVelocity = 8000.;
        this->minVelocity = 900.;
        this->maxAltitude = 112.;
        this->minAltitude = 10.;
    }
};

struct HelicopterInfo: public AirInfo
{
    HelicopterInfo()
    {
        this->type = ObjectCategory::Helicopter;
        this->category = "Air";
        this->value = "Helicopter";
        this->maxVelocity = 400.;
        this->minVelocity = 200.;
        this->maxAltitude = 12.;
        this->minAltitude = 5.;
    }
};

struct MissileInfo: public AirInfo
{
    MissileInfo()
    {
        this->type = ObjectCategory::Missile;
        this->category = "Air";
        this->value = "Missile";
        this->maxVelocity = 3700.;
        this->minVelocity = 1000.;
        this->maxAltitude = 500.;
        this->minAltitude = 300.;
    }
};

struct CargoInfo: public AirInfo
{
    CargoInfo()
    {
        this->type = ObjectCategory::Cargo;
        this->category = "Air";
        this->value = "Cargo Aircraft";
        this->maxVelocity = 1400.;
        this->minVelocity = 800.;
        this->maxAltitude = 20.;
        this->minAltitude = 10.;
    }
};

struct BoeingInfo: public AirInfo
{
    BoeingInfo()
    {
        this->type = ObjectCategory::Boeing;
        this->category = "Air";
        this->value = "Boeing Airplane";
        this->maxVelocity = 1400.;
        this->minVelocity = 800.;
        this->maxAltitude = 20.;
        this->minAltitude = 10.;
    }
};

struct FighterInfo: public AirInfo
{
    FighterInfo()
    {
        this->type = ObjectCategory::Fighter;
        this->category = "Air";
        this->value = "Fighter Jet";
        this->maxVelocity = 1400.;
        this->minVelocity = 800.;
        this->maxAltitude = 20.;
        this->minAltitude = 10.;
    }
};

struct LandInfo: public Enum<ObjectCategory>, public ObjectInfo
{
    LandInfo()
    {
        this->category = "Land";
        this->value = "Land";
        this->maxVelocity = 500.;
        this->minVelocity = 40.;
    }
};

struct NavalInfo: public Enum<ObjectCategory>, public ObjectInfo
{
    NavalInfo()
    {
        this->category = "Naval";
        this->value = "Naval";
        this->maxVelocity = 800.0;
        this->minVelocity = 100.0;
    }
};

struct OnWaterInfo: public NavalInfo
{
    double maxDepth = 0.0;
    double minDepth = 0.0;
    OnWaterInfo()
    {
        this->category = "Naval";
        this->value = "OnWater";
    }
};

struct UnderwaterInfo: public NavalInfo
{
    double maxDepth = 6000.0;
    double minDepth = 0.0;
    UnderwaterInfo()
    {
        this->category = "Naval";
        this->value = "Underwater";
    }
};




#endif // OBJECTTYPE_H

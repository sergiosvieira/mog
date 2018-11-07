#ifndef __OBJECT_CATEGORY_H__
#define __OBJECT_CATEGORY_H__

#include "enum.h"
#include <map>

/*!
 * \brief The ObjectCategory enum
 */
enum class ObjectCategory
{
    PassengerAirPlane,
    ExportationAirPlane,
    Helicopter,
    Missile,
    Fighter,
    Cargo,
    Battletank,
    Vehicle,
    Infantry,
    FishingShip,
    NavalShip,
    Submarine,
    UnderwaterRobot
};

/*!
  * \brief map of string to object category
  */
static std::map<std::string, ObjectCategory> mapStrType =
{
    {"Passenger Airplane", ObjectCategory::PassengerAirPlane},
    {"Exportation Airplane", ObjectCategory::ExportationAirPlane},
    {"Helicopter", ObjectCategory::Helicopter},
    {"Missile", ObjectCategory::Missile},
    {"Fighter Jet", ObjectCategory::Fighter},
    {"Cargo Aircraft", ObjectCategory::Cargo},
    {"Battle Tank", ObjectCategory::Battletank},
    {"Reconnaissance Vehicle", ObjectCategory::Vehicle},
    {"Infantry Fighting", ObjectCategory::Infantry},
    {"Fishing Ship", ObjectCategory::FishingShip},
    {"Naval Ship", ObjectCategory::NavalShip},
    {"Submarine", ObjectCategory::Submarine},
    {"Underwater Robot", ObjectCategory::UnderwaterRobot}
};

/*!
 * \brief map of object category to string
 */
static std::map<ObjectCategory, std::string> mapTypeStr =
{
    {ObjectCategory::PassengerAirPlane, "Passenger Airplane"},
    {ObjectCategory::ExportationAirPlane, "Exportation Airplane"},
    {ObjectCategory::Helicopter, "Helicopter"},
    {ObjectCategory::Missile, "Missile"},
    {ObjectCategory::Fighter, "Fighter Jet"},
    {ObjectCategory::Cargo, "Cargo Aircraft"},
    {ObjectCategory::Battletank, "Battle Tank"},
    {ObjectCategory::Vehicle, "Reconnaissance Vehicle"},
    {ObjectCategory::Infantry, "Infantry Fighting"},
    {ObjectCategory::FishingShip, "Fishing Ship"},
    {ObjectCategory::NavalShip, "Naval Ship"},
    {ObjectCategory::Submarine, "Submarine"},
    {ObjectCategory::UnderwaterRobot, "Underwater Robot"},
};

/*!
 * \brief The ObjectInfo struct
 */
struct ObjectInfo
{
    double maxVelocity = 0.0;
    double minVelocity = 0.0;
    double maxAcceleration = 99.0;
    double minAcceleration = 1.0;
};

/*!
 * \brief The AirObject struct
 */
struct AirObject: public Enum<ObjectCategory>, public ObjectInfo
{
    double maxAltitude = 0.0;
    double minAltitude = 0.0;
    AirObject()
    {
        this->category = "Air";
        this->value = "Air";
    }
};

/*!
 * \brief The LandObject struct
 */
struct LandObject: public Enum<ObjectCategory>, public ObjectInfo
{
    LandObject()
    {
        this->category = "Land";
        this->value = "Land";
        this->maxVelocity = 500.;
        this->minVelocity = 40.;
    }
};

/*!
 * \brief The WaterObject struct
 */
struct WaterObject: public Enum<ObjectCategory>, public ObjectInfo
{
    WaterObject()
    {
        this->category = "Water";
        this->value = "Water";
        this->maxVelocity = 800.0;
        this->minVelocity = 100.0;
    }
};

// Air Objects
/*!
 * \brief The AirForceObject struct
 */
struct AirForceObject: public AirObject
{
    AirForceObject()
    {
        this->type = ObjectCategory::Fighter;
        this->value = "Air Force";
    }
};

/*!
 * \brief The FighterJetObject struct
 */
struct FighterJetObject: public AirForceObject
{
    FighterJetObject()
    {
        this->type = ObjectCategory::Fighter;
        this->value = mapTypeStr[this->type];
        this->maxVelocity = 1400.;
        this->minVelocity = 800.;
        this->maxAltitude = 20.;
        this->minAltitude = 10.;
    }
};

/*!
 * \brief The CargoObject struct
 */
struct CargoObject: public AirForceObject
{
    CargoObject()
    {
        this->type = ObjectCategory::Cargo;
        this->value = mapTypeStr[this->type];
        this->maxVelocity = 1400.;
        this->minVelocity = 800.;
        this->maxAltitude = 20.;
        this->minAltitude = 10.;
    }
};

/*!
 * \brief The HelicopterObject struct
 */
struct HelicopterObject: public AirObject
{
    HelicopterObject()
    {
        this->type = ObjectCategory::Helicopter;
        this->value = mapTypeStr[this->type];
        this->maxVelocity = 400.;
        this->minVelocity = 200.;
        this->maxAltitude = 12.;
        this->minAltitude = 5.;
    }
};

/*!
 * \brief The MissileObject struct
 */
struct MissileObject: public AirObject
{
    MissileObject()
    {
        this->type = ObjectCategory::Missile;
        this->value = mapTypeStr[this->type];
        this->maxVelocity = 3700.;
        this->minVelocity = 1000.;
        this->maxAltitude = 500.;
        this->minAltitude = 300.;
    }
};

/*!
 * \brief The CommercialAirplaneObject struct
 */
struct CommercialAirplaneObject: public AirObject
{
    CommercialAirplaneObject()
    {
        this->type = ObjectCategory::PassengerAirPlane;
        this->value = "Commercial Airplane";
    }
};

/*!
 * \brief The PassengerAirplaneObject struct
 */
struct PassengerAirplaneObject: public CommercialAirplaneObject
{
    PassengerAirplaneObject()
    {
        this->value = mapTypeStr[this->type];
        this->maxVelocity = 1400.;
        this->minVelocity = 800.;
        this->maxAltitude = 20.;
        this->minAltitude = 10.;
    }
};

/*!
 * \brief The ExportationAirplaneObject struct
 */
struct ExportationAirplaneObject: public CommercialAirplaneObject
{
    ExportationAirplaneObject()
    {
        this->value = mapTypeStr[this->type];
        this->maxVelocity = 1400.;
        this->minVelocity = 800.;
        this->maxAltitude = 20.;
        this->minAltitude = 10.;
    }
};


// Land Objects
/*!
 * \brief The BattleTankObject struct
 */
struct BattleTankObject: public LandObject
{
    BattleTankObject()
    {
        this->value = mapTypeStr[this->type];
        this->maxVelocity = 60.;
        this->minVelocity = 1.;
    }
};

/*!
 * \brief The ReconnaissanceVehicleObject struct
 */
struct ReconnaissanceVehicleObject: public LandObject
{
    ReconnaissanceVehicleObject()
    {
        this->value = mapTypeStr[this->type];
        this->maxVelocity = 150.;
        this->minVelocity = 1.;
    }
};

/*!
 * \brief The InfantryFightingObject struct
 */
struct InfantryFightingObject: public LandObject
{
    InfantryFightingObject()
    {
        this->value = mapTypeStr[this->type];
        this->maxVelocity = 150.;
        this->minVelocity = 1.;
    }
};

// Water Objects
/*!
 * \brief The SurfaceShipObject struct
 */
struct SurfaceShipObject: public WaterObject
{
    double maxDepth = 0.0;
    double minDepth = 0.0;
    SurfaceShipObject()
    {
        this->type = ObjectCategory::FishingShip;
        this->category = "Water";
        this->value = "Surface Ship";
    }
};

/*!
 * \brief The FishingShipObject struct
 */
struct FishingShipObject: public SurfaceShipObject
{
    FishingShipObject()
    {
        this->value = mapTypeStr[this->type];
    }
};

/*!
 * \brief The NavalShipObject struct
 */
struct NavalShipObject: public SurfaceShipObject
{
    NavalShipObject()
    {
        this->type = ObjectCategory::NavalShip;
        this->value = mapTypeStr[this->type];
    }
};

/*!
 * \brief The UnderwaterObject struct
 */
struct UnderwaterObject: public WaterObject
{
    double maxDepth = 6000.0;
    double minDepth = 0.0;
    UnderwaterObject()
    {
        this->type = ObjectCategory::Submarine;
        this->category = "Water";
        this->value = "Underwater";
    }
};

/*!
 * \brief The SubmarineObject struct
 */
struct SubmarineObject: public UnderwaterObject
{

    SubmarineObject()
    {
        this->value = mapTypeStr[this->type];
    }
};

/*!
 * \brief The UnderwaterRobotObject struct
 */
struct UnderwaterRobotObject: public WaterObject
{

    UnderwaterRobotObject()
    {
        this->type = ObjectCategory::UnderwaterRobot;
        this->value = mapTypeStr[this->type];
    }
};

#endif // __OBJECT_CATEGORY_H__

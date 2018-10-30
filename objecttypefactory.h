#ifndef OBJECTTYPEFACTORY_H
#define OBJECTTYPEFACTORY_H

#include "object.h"
#include "objecttype.h"
#include <functional>

class ObjectFactory
{
    using FuncMap = std::map<ObjectCategory, std::function<ObjectInfo *()>>;
    static FuncMap map_;
    template<class T>
    static T *makeObject()
    {
        return new T();
    }
public:
    static ObjectInfo *make(ObjectCategory category)
    {
        if (map_.find(category) == map_.end()) return nullptr;
        return map_[category]();
    }
};

ObjectFactory::FuncMap ObjectFactory::map_ = {
    {ObjectCategory::PassengerAirPlane, makeObject<PassengerAirplaneObject>},
    {ObjectCategory::ExportationAirPlane, makeObject<ExportationAirplaneObject>},
    {ObjectCategory::Missile, makeObject<MissileObject>},
    {ObjectCategory::Helicopter, makeObject<HelicopterObject>},
    {ObjectCategory::Fighter, makeObject<FighterJetObject>},
    {ObjectCategory::Cargo, makeObject<CargoObject>},
    {ObjectCategory::Battletank, makeObject<BattleTankObject>},
    {ObjectCategory::Vehicle, makeObject<ReconnaissanceVehicleObject>},
    {ObjectCategory::Infantry, makeObject<InfantryFightingObject>},
    {ObjectCategory::FishingShip, makeObject<FishingShipObject>},
    {ObjectCategory::NavalShip, makeObject<NavalShipObject>},
    {ObjectCategory::Submarine, makeObject<SubmarineObject>},
    {ObjectCategory::UnderwaterRobot, makeObject<UnderwaterRobotObject>}
};


#endif // OBJECTTYPEFACTORY_H

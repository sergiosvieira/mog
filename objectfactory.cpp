#include "objectfactory.h"
#include "objectcategory.h"

/*!
 * \brief map of object category to method
 */
const ObjectFactory::FuncMap ObjectFactory::map_ = {
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

ObjectInfo *ObjectFactory::make(ObjectCategory category)
{
    if (map_.find(category) == map_.end()) return nullptr;
    return map_.at(category)();
}

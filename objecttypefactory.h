#ifndef OBJECTTYPEFACTORY_H
#define OBJECTTYPEFACTORY_H

#include "objecttype.h"

class ObjectInfoFactory
{
public:
    static AirplaneInfo *makeAirplane()
    {
        return new AirplaneInfo();
    }
    static HelicopterInfo *makeHelicopter()
    {
        return new HelicopterInfo();
    }
    static MissileInfo *makeMissile()
    {
        return new MissileInfo();
    }
    static CargoInfo *makeCargo()
    {
        return new CargoInfo();
    }
    static FighterInfo *makeFighter()
    {
        return new FighterInfo();
    }
    static BoeingInfo *makeBoeing()
    {
        return new BoeingInfo();
    }
    static OnWaterInfo *makeOnWater()
    {
        return new OnWaterInfo();
    }
    static UnderwaterInfo *makeUnderwater()
    {
        return new UnderwaterInfo();
    }
};

#endif // OBJECTTYPEFACTORY_H

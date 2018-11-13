#ifndef MAPCOLOR_H
#define MAPCOLOR_H

#include "objectcategory.h"
#include <QBrush>

static std::map<ObjectCategory, QBrush> mapTypeColor =
{
    {ObjectCategory::PassengerAirPlane, QBrush("#c0d732", Qt::SolidPattern)},
    {ObjectCategory::ExportationAirPlane, QBrush("#000bc2", Qt::SolidPattern)},
    {ObjectCategory::Helicopter, QBrush("#61e348", Qt::SolidPattern)},
    {ObjectCategory::Missile, QBrush("#f20569", Qt::SolidPattern)},
    {ObjectCategory::Fighter, QBrush("#4c621a", Qt::SolidPattern)},
    {ObjectCategory::Cargo, QBrush("#a032a0", Qt::SolidPattern)},
    {ObjectCategory::Battletank, QBrush("#709398", Qt::SolidPattern)},
    {ObjectCategory::Vehicle, QBrush("#cdbab4", Qt::SolidPattern)},
    {ObjectCategory::Infantry, QBrush("#de930c", Qt::SolidPattern)},
    {ObjectCategory::FishingShip, QBrush("#de930c", Qt::SolidPattern)},
    {ObjectCategory::NavalShip, QBrush("#d32eb1", Qt::SolidPattern)},
    {ObjectCategory::Submarine, QBrush("#d65357", Qt::SolidPattern)},
    {ObjectCategory::UnderwaterRobot, QBrush("#dc1341", Qt::SolidPattern)}
};

#endif // MAPCOLOR_H

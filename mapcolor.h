#ifndef MAPCOLOR_H
#define MAPCOLOR_H

#include "objectcategory.h"

static std::map<ObjectCategory, QBrush> mapTypeColor =
{
    {ObjectCategory::PassengerAirPlane, QBrush("#1494b8", Qt::SolidPattern)},
    {ObjectCategory::ExportationAirPlane, QBrush("#24a4b8", Qt::SolidPattern)},
    {ObjectCategory::Helicopter, QBrush("#34b4b8", Qt::SolidPattern)},
    {ObjectCategory::Missile, QBrush("#44c4b8", Qt::SolidPattern)},
    {ObjectCategory::Fighter, QBrush("#54d4b8", Qt::SolidPattern)},
    {ObjectCategory::Cargo, QBrush("#64e4b8", Qt::SolidPattern)},
    {ObjectCategory::Battletank, QBrush("#74f4b8", Qt::SolidPattern)},
    {ObjectCategory::Vehicle, QBrush("#8494b9", Qt::SolidPattern)},
    {ObjectCategory::Infantry, QBrush("#9494ba", Qt::SolidPattern)},
    {ObjectCategory::FishingShip, QBrush("#a494bb", Qt::SolidPattern)},
    {ObjectCategory::NavalShip, QBrush("#b494bc", Qt::SolidPattern)},
    {ObjectCategory::Submarine, QBrush("#c494bd", Qt::SolidPattern)},
    {ObjectCategory::UnderwaterRobot, QBrush("#d494be", Qt::SolidPattern)}
};

#endif // MAPCOLOR_H

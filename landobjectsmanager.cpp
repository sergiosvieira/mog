#include "landobjectsmanager.h"

#include <QRect>
#include "objectfactory.h"
#include "objectgenerator.h"
#include "taticalmovingobject.h"
#include "mapcolor.h"
#include "collectedeach.h"
#include "land.h"

void LandObjectsManager::draw(QPainter &painter)
{
    QRect r(-2, -2, 4, 4);
    painter.drawRect(r);
}

void LandObjectsManager::add(ObjectCategory category, DistributionType distribution, unsigned int initialInstant, unsigned int lifeTime, CollectedEachInterface *collected)
{
    if (landObjectsType.count(category))
    {
        LandObject *landObject = (LandObject*)(ObjectFactory::make(category));
        TaticalMovingObject *object = ObjectGenerator::generate(World, category, distribution,
                                                                landObject->maxVelocity, landObject->minVelocity,
                                                                landObject->maxAcceleration, landObject->minAcceleration,
                                                                initialInstant, lifeTime);
        this->objMap[object->getID()] = object;
        delete landObject;
    }
}

std::string LandObjectsManager::str()
{

}

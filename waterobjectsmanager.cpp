#include "waterobjectsmanager.h"

#include <QRect>
#include "objectfactory.h"
#include "objectgenerator.h"
#include "taticalmovingobject.h"
#include "mapcolor.h"

void WaterObjectsManager::add(ObjectCategory category, DistributionType distribution, unsigned int initialInstant, unsigned int lifeTime, CollectedEachInterface *collected)
{
    if (waterObjectsType.count(category))
    {
        WaterObject *water = (WaterObject*)(ObjectFactory::make(category));
        TaticalMovingObject *object = ObjectGenerator::generate(World, category, distribution,
                                                                water->maxVelocity, water->minVelocity,
                                                                water->maxAcceleration, water->minAcceleration,
                                                                initialInstant, lifeTime);
        object->setCollected(collected);
        this->objMap[object->getID()] = object;
        delete water;
    }
}

void WaterObjectsManager::draw(QPainter& painter)
{
    painter.drawEllipse(-2, -2, 4, 4);
}

std::string WaterObjectsManager::str()
{

}

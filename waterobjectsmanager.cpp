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

void WaterObjectsManager::del(unsigned int id)
{
    auto it = this->objMap.find(id);
    if (it != this->objMap.end())
    {
        if (it->second) delete it->second;
        this->objMap.erase(it);
    }
}

void WaterObjectsManager::show(unsigned int id, QImage *image)
{
    auto it = this->objMap.find(id);
    if (it == this->objMap.end()) return;
    if (image)
    {
        TaticalMovingObject *object = it->second;
        if (!object) return;
        QPainter painter(image);
        double x = object->getPosition().getX();
        double y = object->getPosition().getY();
        QBrush brush = mapTypeColor[object->getType()];
        painter.setBrush(brush);
        painter.drawEllipse(x, y, 4, 4);
    }
}

void WaterObjectsManager::update(unsigned int id)
{

}

#include "landobjectsmanager.h"

#include <QRect>
#include "objectfactory.h"
#include "objectgenerator.h"
#include "taticalmovingobject.h"
#include "mapcolor.h"
#include "collectedeach.h"

void LandObjectsManager::add(ObjectCategory category, DistributionType distribution, unsigned int initialInstant, unsigned int lifeTime, CollectedEachInterface *collected)
{
    if (landObjectsType.count(category))
    {
        LandObject *land = (LandObject*)(ObjectFactory::make(category));
        TaticalMovingObject *object = ObjectGenerator::generate(World, category, distribution,
                                                                land->maxVelocity, land->minVelocity,
                                                                land->maxAcceleration, land->minAcceleration,
                                                                initialInstant, lifeTime);
        this->objMap[object->getID()] = object;
        delete land;
    }
}

void LandObjectsManager::del(unsigned int id)
{
    auto it = this->objMap.find(id);
    if (it != this->objMap.end())
    {
        if (it->second) delete it->second;
        this->objMap.erase(it);
    }
}

void LandObjectsManager::show(unsigned int id, QImage *image)
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
        QRect r(x, y, 4, 4);
        painter.drawRect(r);    }
}

void LandObjectsManager::update(unsigned int id)
{

}

std::string LandObjectsManager::str()
{

}

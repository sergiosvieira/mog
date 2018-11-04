#include "airobjectsmanager.h"

#include <QRect>
#include "objectfactory.h"
#include "objectgenerator.h"
#include "taticalmovingobject.h"
#include "mapcolor.h"


void AirObjectsManager::add(ObjectCategory category,
                            DistributionType distribution,
                            unsigned int initialInstant,
                            unsigned int lifeTime)
{
    if (airObjectsType.count(category))
    {
        AirObject *air = (AirObject*)(ObjectFactory::make(category));
        TaticalMovingObject *object = ObjectGenerator::generate(World, category, distribution,
                                                                air->maxVelocity, air->minVelocity,
                                                                air->maxAcceleration, air->minAcceleration,
                                                                initialInstant, lifeTime);
        this->objMap[object->getID()] = object;
        delete air;
    }
}

void AirObjectsManager::del(unsigned int id)
{
    auto it = this->objMap.find(id);
    if (it != this->objMap.end())
    {
        if (it->second) delete it->second;
        this->objMap.erase(it);
    }
}

void AirObjectsManager::show(unsigned int id, QImage *image)
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
        static const QPoint arrow[3] =
        {
            QPoint(3, 3),
            QPoint(-3, 3),
            QPoint(0, -3)
        };
        painter.translate(x, y);
        painter.drawConvexPolygon(arrow, 3);
    }
}

void AirObjectsManager::update(unsigned int id)
{

}

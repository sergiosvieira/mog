#include "airobjectsmanager.h"

#include <QRect>
#include "objectfactory.h"
#include "objectgenerator.h"
#include "taticalmovingobject.h"
#include "mapcolor.h"


void AirObjectsManager::add(ObjectCategory category,
                            DistributionType distribution,
                            unsigned int initialInstant,
                            unsigned int lifeTime,
                            CollectedEachInterface *collected)
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

std::string AirObjectsManager::str()
{
    for (auto kv: this->objMap)
    {
        auto object = kv.second;
        for (int t = 0; t < this->getEndInstant(); ++t)
        {
            Pair p = this->dataManager.getData(t, object->getID());
            Coordinates position = p.first;
            Vector velocity = p.second;
            QString patternStr = QString::fromStdString(Pattern::stringFromPattern(object->getPattern(t)));
            out << t
                << ","
                << object2name(object)
                << ","
                << position.getX()
                << ","
                << position.getY()
                << ","
                << position.getZ()
                << ","
                << velocity.getX()
                << ","
                << velocity.getY()
                << ","
                << velocity.getZ()
                << ","
                << object->getAcceleraton().getX()
                << ","
                << object->getAcceleraton().getY()
                << ","
                << object->getAcceleraton().getZ()
                << ","
                << objectAltitude(object).first // max altitude
                << ","
                << objectAltitude(object).second // min altitude
                << ","
                << objectDepth(object).first // max depth
                << ","
                << objectDepth(object).second // min depth
                << ","
                << object->getInitialTime()
                << ","
                << object->getEndTime()
                << ","
                << t
                << ","
                << patternStr;
             out << "\r\n";
        }
    }

}

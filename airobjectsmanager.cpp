#include "airobjectsmanager.h"

#include <QRect>
#include <sstream>
#include "objectfactory.h"
#include "objectgenerator.h"
#include "taticalmovingobject.h"
#include "mapcolor.h"
#include "air.h"
#include "objectgenerator.h"
#include "cg.h"

void AirObjectsManager::draw(QPainter &painter)
{
    static const QPoint arrow[3] =
    {
        QPoint(3, 3),
        QPoint(-3, 3),
        QPoint(0, -3)
    };
    painter.drawConvexPolygon(arrow, 3);
}

void AirObjectsManager::add(ObjectCategory category,
                            DistributionType distribution,
                            unsigned int initialInstant,
                            unsigned int lifeTime,
                            CollectedEachInterface *collected)
{
    if (airObjectsType.count(category))
    {
        AirObject *airObj = (AirObject*)(ObjectFactory::make(category));
        TaticalMovingObject *object = ObjectGenerator::generate(World, category, distribution,
                                                                airObj->maxVelocity, airObj->minVelocity,
                                                                airObj->maxAcceleration, airObj->minAcceleration,
                                                                initialInstant, lifeTime);
        Air *air = dynamic_cast<Air*>(object);
        double minAltitude = ObjectGenerator::random(airObj->minAltitude, airObj->maxAltitude);
        double maxAltitude = ObjectGenerator::random(minAltitude, airObj->maxAltitude);
        air->setMaxAltitude(maxAltitude);
        air->setMinAltitude(minAltitude);
        this->objMap[object->getID()] = object;
        delete airObj;
    }
}

std::string AirObjectsManager::str()
{
    std::stringstream out;
    for (auto kv: this->objMap)
    {
        Air *object = dynamic_cast<Air *>(kv.second);
        ObjectCategory category = object->getType();
        for (int t = 0; t < this->getEndInstant(); ++t)
        {
            Pair p = this->dataManager.getData(t, object->getID());
            Coordinates position = p.first;
            Vector velocity = p.second;

            std::string patternStr = Pattern::stringFromPattern(object->getPattern(t));
            out << t
                << ","
                << mapTypeStr[category]
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
                << object->getMaxAltitude()
                << ","
                << object->getMinAltitude()
                << ","
                << 0
                << ","
                << 0
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
    return out.str();
}

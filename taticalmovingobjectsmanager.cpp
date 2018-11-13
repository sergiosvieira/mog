#include "taticalmovingobjectsmanager.h"

#include "taticalmovingobject.h"
#include "cg.h"
#include "mapcolor.h"
#include <QPainter>

const QRect TaticalMovingObjectsManager::World{0, 0, 192000, 192000};


TaticalMovingObjectsManager::TaticalMovingObjectsManager()
{

}

TaticalMovingObjectsManager::~TaticalMovingObjectsManager()
{
    this->clearAll();
}

void TaticalMovingObjectsManager::del(unsigned int id)
{
    auto it = this->objMap.find(id);
    if (it != this->objMap.end())
    {
        if (it->second) delete it->second;
        this->objMap.erase(it);
    }
}

void TaticalMovingObjectsManager::show(unsigned int id, unsigned int instant, QImage *image)
{
    auto it = this->objMap.find(id);
    if (it == this->objMap.end()) return;
    if (image)
    {
        TaticalMovingObject *object = it->second;
        if (!object) return;
        QPainter painter(image);
        Pair p = this->dataManager.getData(instant, id);
        QPointF point = CG::w2vp(p.first);
        QBrush brush = mapTypeColor[object->getType()];
        painter.setBrush(brush);
        painter.translate(point.x(), point.y());
        this->draw(painter);
    }
}

void TaticalMovingObjectsManager::update(unsigned int id)
{
    auto object = this->objMap[id];
    for (unsigned int t = object->getInitialTime(); t < object->getEndTime(); ++t)
    {
        Coordinates newPosition(0.0, 0.0, 0.0);
        Vector veloc = object->getVelocity();
        if (t == object->getInitialTime())
        {
            newPosition = object->getPosition();
        }
        else
        {
            MovingPattern pattern = object->getPattern(t);
            Vector accel = object->getAcceleraton();
            if (pattern == MovingPattern::Const_Velocity)
            {
                accel = Vector{0., 0., 0.};
            }
            else if (pattern == MovingPattern::Const_Deceleration)
            {
                accel *= -1.0;
            }
            Pair p = dataManager.getData(t - 1, object->getID());
            newPosition = p.first;
            veloc = p.second;
            veloc += accel;
            newPosition.setX(newPosition.getX() + veloc.getX() * object->getDirection().getX());
            newPosition.setY(newPosition.getY() + veloc.getY() * object->getDirection().getY());
            newPosition.setZ(newPosition.getZ() + veloc.getZ() * object->getDirection().getZ());
        }
        if (t >= object->getInitialTime() && t <= object->getEndTime())
        {
            dataManager.setData(t, object->getID(), std::make_pair(newPosition, veloc));
        }
        else
        {
            Pair p = dataManager.getData(t, object->getID());
            dataManager.setData(t, object->getID(), std::make_pair(p.first, Vector(0.0, 0.0, 0.0)));
        }
    }
}

void TaticalMovingObjectsManager::clearAll()
{
    for (std::map<unsigned int, TaticalMovingObject*>::iterator it = this->objMap.begin();
         it != this->objMap.end(); ++it)
    {
        if ((*it).second != nullptr) delete (*it).second;
        this->objMap.erase(it);
    }
    this->objMap.clear();
}

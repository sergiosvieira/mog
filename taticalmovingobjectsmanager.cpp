#include "taticalmovingobjectsmanager.h"

#include "taticalmovingobject.h"

const QRect TaticalMovingObjectsManager::World{0, 0, 192000, 192000};


TaticalMovingObjectsManager::TaticalMovingObjectsManager()
{

}

TaticalMovingObjectsManager::~TaticalMovingObjectsManager()
{
    for (std::map<unsigned int, TaticalMovingObject*>::iterator it = this->objMap.begin();
         it != this->objMap.end(); ++it)
    {
        if ((*it).second != nullptr) delete (*it).second;
        this->objMap.erase(it);
    }
}

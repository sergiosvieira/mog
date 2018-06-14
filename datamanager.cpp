#include "datamanager.h"

#include "object.h"

DataManager::DataManager()
{

}

void DataManager::setData(int time, int objectID, const Pair& pair_)
{
    this->data[time][objectID] = pair_;
}

Pair DataManager::getData(int time, int objectID)
{
    if (this->data.count(time) > 0)
    {
        if (this->data[time].count(objectID) > 0)
        {
            return this->data[time][objectID];
        }
    }
    return Pair();
}

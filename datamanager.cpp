#include "datamanager.h"

#include "object.h"

DataManager::DataManager()
{

}

void DataManager::setData(int time, int objectID, const Coordinates& position)
{
    this->data[time][objectID] = position;
}

Coordinates DataManager::getData(int time, int objectID)
{
    if (this->data.count(time) > 0)
    {
        if (this->data[time].count(objectID) > 0)
        {
            return this->data[time][objectID];
        }
    }
    return Coordinates();
}

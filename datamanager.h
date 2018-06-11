#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include <map>
#include "vector.h"
#include "coordinates.h"

class Object;

using Data = std::map<int, std::map<int, Coordinates>>;

class DataManager
{
protected:
    Data data;
public:
    DataManager();
    void setData(int time, int objectID, const Coordinates& Coordinates);
    Coordinates getData(int time, int objectID);
};


#endif // DATAMANAGER_H

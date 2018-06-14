#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include <map>
#include <utility>
#include "vector.h"
#include "coordinates.h"

class Object;
// Coordinates, Velocity
using Pair = std::pair<Coordinates, Vector>;
using Data = std::map<int, std::map<int, Pair>>;

class DataManager
{
protected:
    Data data;
public:
    DataManager();
    void setData(int time, int objectID, const Pair& pair_);
    Pair getData(int time, int objectID);
};


#endif // DATAMANAGER_H

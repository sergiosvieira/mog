#ifndef OBJECTFEATURE_H
#define OBJECTFEATURE_H

#include "object.h"

class ObjectFeature
{
    double maxVelocity = 0.0;
    double minVelocity = 0.0;
    double maxAltitude = 0.0;
    double minAltitude = 0.0;
    MovingPattern movingPattern;
    int timeRange;
public:
};

#endif // OBJECTFEATURE_H

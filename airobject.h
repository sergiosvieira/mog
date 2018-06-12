#ifndef AIROBJECT_H
#define AIROBJECT_H

#include "object.h"

class AirObject: public Object
{
    double maxAltitude = 0.0;
    double minAltitude = 0.0;
public:
    AirObject();
    AirObject
    (
        const Coordinates& position,
        const Vector& velocity,
        unsigned int initialTime,
        unsigned int lifeTime,
        const Vector& acceleration,
        ObjectType type
    );
    void setMaxAltitude(double value);
    double getMaxAltitude() const;
    void setMinAltitude(double value);
    double getMinAltitude() const;
};

#endif // AIROBJECT_H

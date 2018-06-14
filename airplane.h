#ifndef AIRPLANE_H
#define AIRPLANE_H

#include "airobject.h"

class AirPlane : public AirObject
{

public:
    AirPlane();
    AirPlane
    (
        const Coordinates& position,
        const Vector& velocity,
        unsigned int initialTime,
        unsigned int lifeTime,
        const Vector &acceleration
    );
};

#endif // AIRPLANE_H

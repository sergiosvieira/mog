#ifndef AIRPLANE_H
#define AIRPLANE_H

#include "object.h"

class AirPlane : public Object
{
public:
    AirPlane();
    AirPlane
    (
        const Coordinates& position,
        const Vector& velocity,
        unsigned int initialTime,
        const Vector &acceleration
    );
};

#endif // AIRPLANE_H

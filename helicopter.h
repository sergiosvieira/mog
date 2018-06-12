#ifndef HELICOPTER_H
#define HELICOPTER_H

#include "object.h"

class Helicopter : public Object
{
public:
    Helicopter();
    Helicopter
    (
        const Coordinates& position,
        const Vector& velocity,
        unsigned int initialTime,
        unsigned int lifeTime,
        const Vector &acceleration
    );
};

#endif // HELICOPTER_H

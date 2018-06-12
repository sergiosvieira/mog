#ifndef SHIP_H
#define SHIP_H

#include "object.h"

class Ship: public Object
{
public:
    Ship();
    Ship
    (
        const Coordinates& position,
        const Vector& velocity,
        unsigned int initialTime,
        unsigned int lifeTime,
        const Vector &acceleration
    );
};

#endif // SHIP_H

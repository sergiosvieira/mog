#ifndef CAR_H
#define CAR_H

#include "object.h"

class Car : public Object
{
public:
    Car();
    Car
    (
        const Coordinates& position,
        const Vector& velocity,
        unsigned int initialTime,
        unsigned int lifeTime,
        const Vector &acceleration
    );
};

#endif // CAR_H

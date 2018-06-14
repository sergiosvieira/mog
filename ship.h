#ifndef SHIP_H
#define SHIP_H

#include "object.h"

class Ship: public Object
{
protected:
    double maxDepth = 0.0;
    double minDepth = 0.0;
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
    void setMaxDepth(double value);
    double getMaxDepth() const;
    void setMinDepth(double value);
    double getMinDepth() const;
};

#endif // SHIP_H

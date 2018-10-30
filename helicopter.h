#ifndef HELICOPTER_H
#define HELICOPTER_H

#include "airobject.h"

class Helicopter : public Air
{
protected:
    double rotationAngle = 0.0;
    unsigned int rotationStart = 0;
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
    void setRotationAngle(double value);
    double getRotationAngle() const;
    void setRotationStart(unsigned int value);
    unsigned int getRotationStart() const;
};

#endif // HELICOPTER_H

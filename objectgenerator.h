#ifndef OBJECTGENERATOR_H
#define OBJECTGENERATOR_H

#include "object.h"
#include <random>
#include <QRect>

static std::random_device rdev{};
static std::mt19937 eng{rdev()};


class ObjectGenerator
{
    static unsigned int id;
public:
    static Object* generate
    (
        const QRect& world,
        ObjectType type,
        double maxVelocity,
        double minVelocity,
        double maxAcceleration,
        double minAcceleration,
        unsigned int initialTime,
        unsigned int lifeTime
     );
};

#endif // OBJECTGENERATOR_H

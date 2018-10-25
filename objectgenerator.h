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
    static Object* generate(
            const QRect& world,
            ObjectType type,
            const DistributionType distributionType,
            double maxVelocity,
            double minVelocity,
            double maxAcceleration,
            double minAcceleration,
            unsigned int initialTime,
            unsigned int lifeTime
            );
    static Object* generateStationObject(
            GraphicsViewType areaType,
            const QRect& world,
            const QRectF &rcWhiteArea,
            ObjectType type,
            const DistributionType distributionType,
            double maxVelocity,
            double minVelocity,
            double maxAcceleration,
            double minAcceleration
            );
private:
    static double random(double min, double max);
    static Vector randomVector(double min, double max);
};

#endif // OBJECTGENERATOR_H

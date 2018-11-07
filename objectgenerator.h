#ifndef OBJECTGENERATOR_H
#define OBJECTGENERATOR_H


#include <random>
#include <QRect>
#include "distributiontype.h"
#include "objectcategory.h"
#include "vector.h"
#include "graphicsview.h"

static std::random_device rdev{};
static std::mt19937 eng{rdev()};

class TaticalMovingObject;

class ObjectGenerator
{
    static unsigned int id;
public:
    static TaticalMovingObject* generate(
            const QRect& world,
            ObjectCategory type,
            const DistributionType distributionType,
            double maxVelocity,
            double minVelocity,
            double maxAcceleration,
            double minAcceleration,
            unsigned int initialTime,
            unsigned int lifeTime
            );
    static TaticalMovingObject* generateStationObject(
            GraphicsViewType areaType,
            const QRect& world,
            const QRectF &rcWhiteArea,
            ObjectCategory type,
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

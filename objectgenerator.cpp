#include "objectgenerator.h"

#include <random>
#include <QRect>
#include <tgmath.h>

#include "airplane.h"
#include "helicopter.h"
#include "land.h"
#include "water.h"

unsigned int ObjectGenerator::id = 0;

static std::uniform_real_distribution<double> distributionUniform(0.0, 1.0);
static std::poisson_distribution<int> distributionPoisson(100);
static std::normal_distribution<double> distributionNormal(0.5, sqrt(0.1 / 12.0));
static std::uniform_real_distribution<double> distributionUniformForSkewed4(-3.0, 1.0);
static std::lognormal_distribution<double> distributionLogNormal(0.0,1.0);

static double distributionSkew(DistributionType type)
{
    double alpha = -4.0;
    double x = distributionUniformForSkewed4(eng);
    double result = 100.0;
    do {
        result = 1.0 / sqrt( 2.0 * 3.1415926) * exp(-x * x / 2.0) * ( 1 + erf(alpha * x / sqrt(2.0))) / 0.7;
        if (type == DistributionType::RightSkewed) {
            result = 1.0 - result;
        }
    } while (result >= 1.0 || result <= 0.0);


    return result;
}

static double distributionUniform01()
{
    double result = 100.0;
    do {
        result = distributionUniform(eng);
    } while (result >= 1.0 || result <= 0.0);

    return result;
}

static double distributionNormal01()
{
    double result = 100.0;
    do {
        result = distributionNormal(eng);
    } while (result >= 1.0 || result <= 0.0);

    return result;
}

static double distributionPoisson01()
{
    double result = 100.0;
    do {
        result = distributionPoisson(eng) / 100.0;
    } while (result >= 1.0 || result <= 0.0);
    return result;
}

double rndMinMax(double min, double max)
{
    std::uniform_real_distribution<double> urd(min, max);
    return urd(eng);
}

void rndCoordinate(const QRectF &rcWorld, const DistributionType type, Coordinates &out)
{
    out.setX(0.0);
    out.setY(0.0);

    switch (type) {
    case DistributionType::Uniform:
        out.setX(distributionUniform01() * rcWorld.width());
        out.setY(distributionUniform01() * rcWorld.height());
        out.setZ(distributionUniform01());
        break;
    case DistributionType::Poisson:
        out.setX(distributionPoisson01() * rcWorld.width());
        out.setY(distributionPoisson01() * rcWorld.height());
        out.setZ(distributionPoisson01());
        break;
    case DistributionType::Gaussian:
        out.setX(distributionNormal01() * rcWorld.width());
        out.setY(distributionNormal01() * rcWorld.height());
        out.setZ(distributionNormal01());
        break;
    case DistributionType::LeftSkewed:
        out.setX(distributionSkew(DistributionType::LeftSkewed) * rcWorld.width());
        out.setY(distributionSkew(DistributionType::LeftSkewed) * rcWorld.height());
        out.setZ(distributionSkew(DistributionType::LeftSkewed));
        break;
    case DistributionType::RightSkewed:
        out.setX(distributionSkew(DistributionType::RightSkewed) * rcWorld.width());
        out.setY(distributionSkew(DistributionType::RightSkewed) * rcWorld.height());
        out.setZ(distributionSkew(DistributionType::RightSkewed));
        break;
    }
}


double ObjectGenerator::random(double min, double max)
{
    double min_ = rndMinMax(min, max);
    double max_ = rndMinMax(min_, max);
    return rndMinMax(min_, max_);
}

Vector ObjectGenerator::randomVector(double min, double max)
{
    return Vector(ObjectGenerator::random(min, max),
                  ObjectGenerator::random(min, max),
                  ObjectGenerator::random(min, max));
}


TaticalMovingObject *ObjectGenerator::generate(const QRectF& world,
                                  ObjectCategory type,
                                  const DistributionType distributionType,
                                  double maxVelocity,
                                  double minVelocity,
                                  double maxAcceleration,
                                  double minAcceleration,
                                  unsigned int initialTime,
                                  unsigned int lifeTime)
{
    TaticalMovingObject * object = nullptr;
    Coordinates randomPosition;
    rndCoordinate(world, distributionType, randomPosition);
    Vector randomVelocity = ObjectGenerator::randomVector(minVelocity, maxVelocity);
    randomVelocity /= 3.6; // Converting km/h to m/s
    Vector randomAcceleration = ObjectGenerator::randomVector(minAcceleration, maxAcceleration);
    double rndX = (rndMinMax(0, 1) < 0.5) ? -1.0 : 1.0;
    double rndY = (rndMinMax(0, 1) < 0.5) ? -1.0 : 1.0;
    double rndZ = (rndMinMax(0, 1) < 0.5) ? -1.0 : 1.0;

    if (landObjectsType.count(type) > 0
            || waterObjectsType.count(type))
    {
        rndZ = 0.0;
        randomPosition.setZ(0.0);
        randomVelocity.setZ(0.0);
        randomAcceleration.setZ(0.0);
    }
    if (airObjectsType.count(type) > 0)
    {
        if (type == ObjectCategory::Helicopter)
        {
            object = new Helicopter(randomPosition, randomVelocity, initialTime, lifeTime, randomAcceleration);
        }
        else
        {
            object = new AirPlane(randomPosition, randomVelocity, initialTime, lifeTime, randomAcceleration);
        }
    }
    else if (landObjectsType.count(type) > 0)
    {
        object = new Land(randomPosition, randomVelocity, initialTime, lifeTime, randomAcceleration);
    }
    else if (waterObjectsType.count(type) > 0
             || underwaterObjectsType.count(type) > 0)
    {
        object = new Water(randomPosition, randomVelocity, initialTime, lifeTime, randomAcceleration);
    }

    QString typeStr = QString::fromStdString(TaticalMovingObject::stringFromType(type));
    QString name = QString("%1_%2").arg(typeStr).arg(id);
    object->setName(name);
    object->setType(type);
    object->setID(id);
    object->setDirection(Vector(rndX, rndY, rndZ));
    id++;
    return object;
}

void rndStationCoordinate(ObjectCategory objType,
                          const QRectF &rcWorld,
                          const QRectF &rcWhiteArea,
                          const DistributionType type,
                          Coordinates &out)
{
    double x = 0.0, y = 0.0, z = 0.0;
    switch (type)
    {
        case DistributionType::Uniform:
            x = distributionUniform01() * rcWorld.width();
            y = distributionUniform01() * rcWorld.height();
            break;
        case DistributionType::Poisson:
            x = distributionPoisson01() * rcWorld.width();
            y = distributionPoisson01() * rcWorld.height();
            break;
        case DistributionType::Gaussian:
            x = distributionNormal01() * rcWorld.width();
            y = distributionNormal01() * rcWorld.height();
            break;
        case DistributionType::LeftSkewed:
            x = distributionSkew(DistributionType::LeftSkewed) * rcWorld.width();
            y = distributionSkew(DistributionType::LeftSkewed) * rcWorld.height();
            break;
        case DistributionType::RightSkewed:
            x = distributionSkew(DistributionType::RightSkewed) * rcWorld.width();
            y = distributionSkew(DistributionType::RightSkewed) * rcWorld.height();
            break;
    }
    out.setX(x);
    out.setY(y);
    if (airObjectsType.count(objType) > 0
            || underwaterObjectsType.count(objType))
    {
        switch (type)
        {
            case DistributionType::Uniform:
                z = distributionUniform01();
                break;
            case DistributionType::Poisson:
                z = distributionPoisson01();
                break;
            case DistributionType::Gaussian:
                z = distributionNormal01();
                break;
            case DistributionType::LeftSkewed:
                z = distributionSkew(DistributionType::LeftSkewed);
                break;
            case DistributionType::RightSkewed:
                z = distributionSkew(DistributionType::RightSkewed);
                break;
        }
    }
    out.setZ(z);
}

TaticalMovingObject* ObjectGenerator::generateStationObject(GraphicsViewType areaType,
                                               const QRectF& world,
                                               const QRectF &rcWhiteArea,
                                               ObjectCategory type, const DistributionType distributionType, double maxVelocity, double minVelocity, double maxAcceleration, double minAcceleration)
{
    double minVelX = rndMinMax(minVelocity, maxVelocity);
    double maxVelX = rndMinMax(minVelX, maxVelocity);
    double minVelY = rndMinMax(minVelocity, maxVelocity);
    double maxVelY = rndMinMax(minVelY, maxVelocity);
    double minVelZ = rndMinMax(minVelocity, maxVelocity);
    double maxVelZ = rndMinMax(minVelZ, maxVelocity);
    if (maxVelX > maxVelocity) maxVelX = maxVelocity;
    if (maxVelY > maxVelocity) maxVelY = maxVelocity;
    if (maxVelZ > maxVelocity) maxVelZ = maxVelocity;
    double minAccelX = rndMinMax(minAcceleration, maxAcceleration);
    double maxAccelX = rndMinMax(minAccelX, maxAcceleration);
    double minAccelY = rndMinMax(minAcceleration, maxAcceleration);
    double maxAccelY = rndMinMax(minAccelY, maxAcceleration);
    double minAccelZ = rndMinMax(minAcceleration, maxAcceleration);
    double maxAccelZ = rndMinMax(minAccelZ, maxAcceleration);
    if (maxAccelX > maxAcceleration) maxAccelX = maxAcceleration;
    if (maxAccelY > maxAcceleration) maxAccelY = maxAcceleration;
    if (maxAccelZ > maxAcceleration) maxAccelZ = maxAcceleration;
    TaticalMovingObject * object = nullptr;
//    Coordinates randomPosition(rndMinMax(0., 1.) * world.width(), rndMinMax(0., 1.) * world.height());
    Coordinates randomPosition;
    QRectF rc(rcWhiteArea);
    if (areaType == GraphicsViewType::NK) {
        rc.setY(rcWhiteArea.y() + rcWhiteArea.height() / 2.0);
        rc.setHeight(rcWhiteArea.height() / 2.0);
        rndStationCoordinate(type, world, rc, distributionType, randomPosition);
    } else {
        rc.setHeight(rcWhiteArea.height() / 2.0);
        rndStationCoordinate(type, world, rc, distributionType, randomPosition);
    }
    Vector randomVelocity(rndMinMax(minVelX, maxVelX), rndMinMax(minVelY, maxVelY), rndMinMax(minVelZ, maxVelZ));
    Vector randomAcceleration(rndMinMax(minAccelX, maxAccelX), rndMinMax(minAccelY, maxAccelY), rndMinMax(minAccelZ, maxAccelZ));
    double rndX = (rndMinMax(0, 1) < 0.5) ? -1.0 : 1.0;
    double rndY = (rndMinMax(0, 1) < 0.5) ? -1.0 : 1.0;
    double rndZ = (rndMinMax(0, 1) < 0.5) ? -1.0 : 1.0;
    if (landObjectsType.count(type) > 0
            || waterObjectsType.count(type) > 0)
    {
        rndZ = 0.0;
        randomPosition.setZ(0.0);
        randomVelocity.setZ(0.0);
        randomAcceleration.setZ(0.0);
    }
    if (airObjectsType.count(type) > 0)
    {
        object = new AirPlane(randomPosition, randomVelocity, 0.0, 0.0, randomAcceleration);
    }
    else if (landObjectsType.count(type) > 0)
    {
        object = new Helicopter(randomPosition, randomVelocity, 0.0, 0.0, randomAcceleration);
    }
    else if (waterObjectsType.count(type) > 0
             || underwaterObjectsType.count(type) > 0)
    {
        object = new Water(randomPosition, randomVelocity, 0.0, 0.0, randomAcceleration);
    }
    object->setID(id++);
    object->setDirection(Vector(rndX, rndY, rndZ));
    object->setAreaType(areaType);
    return object;
}

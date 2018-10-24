#include "objectgenerator.h"

#include <random>
#include <QRect>
#include <tgmath.h>

#include "airplane.h"
#include "helicopter.h"
#include "car.h"
#include "ship.h"

unsigned int ObjectGenerator::id = 0;

static std::uniform_real_distribution<double> distributionUniform(0.0, 1.0);
static std::poisson_distribution<int> distributionPoisson(1.);
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
        result = distributionPoisson(eng);
    } while (result >= 1.0 || result <= 0.0);
    return result;
}

double rndMinMax(double min, double max)
{
    std::uniform_real_distribution<double> urd(min, max);
    return urd(eng);
}

void rndCoordinate(const QRect &rcWorld, const DistributionType type, Coordinates &out)
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



Object *ObjectGenerator::generate(const QRect& world,
                                  ObjectType type,
                                  const DistributionType distributionType,
                                  double maxVelocity,
                                  double minVelocity,
                                  double maxAcceleration,
                                  double minAcceleration,
                                  unsigned int initialTime,
                                  unsigned int lifeTime)
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
    Object * object = nullptr;
//    Coordinates randomPosition(rndMinMax(0., 1.) * world.width(), rndMinMax(0., 1.) * world.height());
    Coordinates randomPosition;
    rndCoordinate(world, distributionType, randomPosition);
    Vector randomVelocity(rndMinMax(minVelX, maxVelX), rndMinMax(minVelY, maxVelY), rndMinMax(minVelZ, maxVelZ));
    Vector randomAcceleration(rndMinMax(minAccelX, maxAccelX), rndMinMax(minAccelY, maxAccelY), rndMinMax(minAccelZ, maxAccelZ));
    double rndX = (rndMinMax(0, 1) < 0.5) ? -1.0 : 1.0;
    double rndY = (rndMinMax(0, 1) < 0.5) ? -1.0 : 1.0;
    double rndZ = (rndMinMax(0, 1) < 0.5) ? -1.0 : 1.0;
//    if (type != ObjectType::AirPlane && type!= ObjectType::Helicopter) {
//        rndZ = 0.0;
//        randomPosition.setZ(0.0);
//        randomVelocity.setZ(0.0);
//        randomAcceleration.setZ(0.0);
//    }
    switch (type)
    {
    case ObjectType::AirPlane:
    case ObjectType::Missile:
    case ObjectType::Cargo:
    case ObjectType::Boing:
    case ObjectType::Fighter:
        object = new AirPlane(randomPosition, randomVelocity, initialTime, lifeTime, randomAcceleration);
        break;
    case ObjectType::Helicopter:
        object = new Helicopter(randomPosition, randomVelocity, initialTime, lifeTime, randomAcceleration);
        break;
    case ObjectType::Land:
        object = new Car(randomPosition, randomVelocity, initialTime, lifeTime, randomAcceleration);
        break;
    case ObjectType::OnWater:
    case ObjectType::Underwater:
        object = new Ship(randomPosition, randomVelocity, initialTime, lifeTime, randomAcceleration);
    }
    object->setID(id++);
    object->setDirection(Vector(rndX, rndY, rndZ));
    return object;
}

void rndStationCoordinate( ObjectType objType, const QRect &rcWorld, const QRectF &rcWhiteArea, const DistributionType type, Coordinates &out)
{
    out.setX(0.0);
    out.setY(0.0);

    double x, y, z;
    switch (objType) {
    case ObjectType::Helicopter:
    case ObjectType::AirPlane: // spread on overall area
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
        break;
    case ObjectType::OnWater: // spread only on blue area
        do {
            switch (type) {
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
        } while (rcWhiteArea.contains(x, y));
        switch (type) {
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
        out.setX(x);
        out.setY(y);
        out.setZ(z);
        break;
    case ObjectType::Land: // spread only on white area
        switch (type) {
        case DistributionType::Uniform:
            out.setX(rcWhiteArea.x() + distributionUniform01() * rcWhiteArea.width());
            out.setY(rcWhiteArea.y() + distributionUniform01() * rcWhiteArea.height());
            out.setZ(distributionUniform01());
            break;
        case DistributionType::Poisson:
            out.setX(rcWhiteArea.x() + distributionNormal01() * rcWhiteArea.width());
            out.setY(rcWhiteArea.y() + distributionNormal01() * rcWhiteArea.height());
            out.setZ(distributionPoisson01());
            break;
        case DistributionType::Gaussian:
            out.setX(rcWhiteArea.x() + distributionNormal01() * rcWhiteArea.width());
            out.setY(rcWhiteArea.y() + distributionNormal01() * rcWhiteArea.height());
            out.setZ(distributionNormal01());
            break;
        case DistributionType::LeftSkewed:
            out.setX(rcWhiteArea.x() + distributionSkew(DistributionType::LeftSkewed) * rcWhiteArea.width());
            out.setY(rcWhiteArea.y() + distributionSkew(DistributionType::LeftSkewed) * rcWhiteArea.height());
            out.setZ(distributionSkew(DistributionType::LeftSkewed));
            break;
        case DistributionType::RightSkewed:
            out.setX(rcWhiteArea.x() + distributionSkew(DistributionType::RightSkewed) * rcWhiteArea.width());
            out.setY(rcWhiteArea.y() + distributionSkew(DistributionType::RightSkewed) * rcWhiteArea.height());
            out.setZ(distributionSkew(DistributionType::RightSkewed));
            break;
        }
        break;
    }
}

Object* ObjectGenerator::generateStationObject(GraphicsViewType areaType, const QRect& world, const QRectF &rcWhiteArea, ObjectType type, const DistributionType distributionType, double maxVelocity, double minVelocity, double maxAcceleration, double minAcceleration)
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
    Object * object = nullptr;
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
    if (type != ObjectType::AirPlane && type!= ObjectType::Helicopter) {
        rndZ = 0.0;
        randomPosition.setZ(0.0);
        randomVelocity.setZ(0.0);
        randomAcceleration.setZ(0.0);
    }
    switch (type)
    {
    case ObjectType::AirPlane:
        object = new AirPlane(randomPosition, randomVelocity, 0.0, 0.0, randomAcceleration);
        break;
    case ObjectType::Helicopter:
        object = new Helicopter(randomPosition, randomVelocity, 0.0, 0.0, randomAcceleration);
        break;
    case ObjectType::Land:
        object = new Car(randomPosition, randomVelocity, 0.0, 0.0, randomAcceleration);
        break;
    case ObjectType::OnWater:
        object = new Ship(randomPosition, randomVelocity, 0.0, 0.0, randomAcceleration);
    }
    object->setID(id++);
    object->setDirection(Vector(rndX, rndY, rndZ));
    object->setAreaType(areaType);
    return object;
}

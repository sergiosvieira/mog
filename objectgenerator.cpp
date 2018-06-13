#include "objectgenerator.h"

#include <random>
#include <QRect>

#include "airplane.h"
#include "helicopter.h""
#include "car.h"
#include "ship.h"

unsigned int ObjectGenerator::id = 0;

double rndMinMax(double min, double max)
{
    std::uniform_real_distribution<double> urd(min, max);
    return urd(eng);
}


Object *ObjectGenerator::generate(const QRect& world, ObjectType type, double maxVelocity, double minVelocity, double maxAcceleration, double minAcceleration, unsigned int initialTime, unsigned int lifeTime)
{
    double minVelX = rndMinMax(minVelocity, maxVelocity);
    double maxVelX = rndMinMax(minVelX, maxVelocity);
    double minVelY = rndMinMax(minVelocity, maxVelocity);
    double maxVelY = rndMinMax(minVelY, maxVelocity);
    if (maxVelX > maxVelocity) maxVelX = maxVelocity;
    if (maxVelY > maxVelocity) maxVelY = maxVelocity;
    double minAccelX = rndMinMax(minAcceleration, maxAcceleration);
    double maxAccelX = rndMinMax(minAccelX, maxAcceleration);
    double minAccelY = rndMinMax(minAcceleration, maxAcceleration);
    double maxAccelY = rndMinMax(minAccelX, maxAcceleration);
    if (maxAccelX > maxAcceleration) maxAccelX = maxAcceleration;
    if (maxAccelY > maxAcceleration) maxAccelY = maxAcceleration;
    Object * object = nullptr;
    Coordinates randomPosition(rndMinMax(0., 1.) * world.width(), rndMinMax(0., 1.) * world.height());
    Vector randomVelocity(rndMinMax(minVelX, maxVelX), rndMinMax(minVelY, maxVelY));
    Vector randomAcceleration(rndMinMax(minAccelX, maxAccelX), rndMinMax(minAccelY, maxAccelY));
    switch (type)
    {
    case ObjectType::AirPlane:
        object = new AirPlane(randomPosition, randomVelocity, initialTime, lifeTime, randomAcceleration);
        break;
    case ObjectType::Helicopter:
        object = new Helicopter(randomPosition, randomVelocity, initialTime, lifeTime, randomAcceleration);
        break;
    case ObjectType::Car:
        object = new Car(randomPosition, randomVelocity, initialTime, lifeTime, randomAcceleration);
        break;
    case ObjectType::Ship:
        object = new Ship(randomPosition, randomVelocity, initialTime, lifeTime, randomAcceleration);
        break;
    }
    object->setID(id++);
    double rndX = (rndMinMax(0, 1) < 0.5) ? -1.0 : 1.0;
    double rndY = (rndMinMax(0, 1) < 0.5) ? -1.0 : 1.0;
    object->setDirection(Vector(rndX, rndY));
    return object;
}

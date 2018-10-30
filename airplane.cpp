#include "airplane.h"

AirPlane::AirPlane()
{
}

AirPlane::AirPlane (const Coordinates &position, const Vector &velocity, unsigned int initialTime,
    unsigned int lifeTime, const Vector& acceleration):
    Air(position, velocity, initialTime, lifeTime, acceleration, ObjectCategory::ExportationAirPlane)
{

}

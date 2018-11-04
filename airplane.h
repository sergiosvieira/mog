#ifndef AIRPLANE_H
#define AIRPLANE_H

#include "air.h"
/*!
 * \brief The AirPlane class
 */
class AirPlane : public Air
{
public:
    /*!
     * \brief AirPlane constructor
     */
    AirPlane();
    /*!
     * \brief AirPlane constructor
     * \param position
     * \param velocity
     * \param initialTime
     * \param lifeTime
     * \param acceleration
     */
    AirPlane(const Coordinates& position, const Vector& velocity, unsigned int initialTime,
        unsigned int lifeTime, const Vector &acceleration);
};

#endif // AIRPLANE_H

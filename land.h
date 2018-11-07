#ifndef CAR_H
#define CAR_H

#include "taticalmovingobject.h"

/*!
 * \brief The Land class
 */
class Land : public TaticalMovingObject
{
public:
    /*!
     * \brief Land constructor
     */
    Land();
    /*!
     * \brief Land constructor
     * \param position
     * \param velocity
     * \param initialTime
     * \param lifeTime
     * \param acceleration
     */
    Land(const Coordinates& position,
        const Vector& velocity,
        unsigned int initialTime,
        unsigned int lifeTime,
        const Vector &acceleration);
};

#endif // CAR_H

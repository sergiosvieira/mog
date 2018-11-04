#ifndef SHIP_H
#define SHIP_H

#include "taticalmovingobject.h"

/*!
 * \brief The Water class
 */
class Water: public TaticalMovingObject
{
protected:
    /*!
     * \brief maxDepth
     */
    double maxDepth = 0.0;
    /*!
     * \brief minDepth
     */
    double minDepth = 0.0;
public:
    /*!
     * \brief Water constructor
     */
    Water();
    /*!
     * \brief Water constructor
     * \param position
     * \param velocity
     * \param initialTime
     * \param lifeTime
     * \param acceleration
     */
    Water(const Coordinates& position,
        const Vector& velocity,
        unsigned int initialTime,
        unsigned int lifeTime,
        const Vector &acceleration);
    /*!
     * \brief setMaxDepth
     * \param value
     */
    void setMaxDepth(double value);
    /*!
     * \brief getMaxDepth
     * \return
     */
    double getMaxDepth() const;
    /*!
     * \brief setMinDepth
     * \param value
     */
    void setMinDepth(double value);
    /*!
     * \brief getMinDepth
     * \return
     */
    double getMinDepth() const;
};

#endif // SHIP_H

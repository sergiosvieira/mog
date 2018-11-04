#ifndef __AIR_OBJ_H__
#define __AIR_OBJ_H__

#include "taticalmovingobject.h"
#include "objectcategory.h"

/*!
 * \brief The Air class
 */
class Air: public TaticalMovingObject
{
    /*!
     * \brief maxAltitude
     */
    double maxAltitude = 0.0;
    /*!
     * \brief minAltitude
     */
    double minAltitude = 0.0;
public:
    /*!
     * \brief Air Constructor
     */
    Air ();
    /*!
     * \brief Air Constructor
     * \param position
     * \param velocity
     * \param initialTime
     * \param lifeTime
     * \param acceleration
     * \param type
     */
    Air (const Coordinates& position,
        const Vector& velocity,
        unsigned int initialTime,
        unsigned int lifeTime,
        const Vector& acceleration,
        ObjectCategory type);
    /*!
     * \brief setMaxAltitude
     * \param value
     */
    void setMaxAltitude(double value);
    /*!
     * \brief getMaxAltitude
     * \return
     */
    double getMaxAltitude() const;
    /*!
     * \brief setMinAltitude
     * \param value
     */
    void setMinAltitude(double value);
    /*!
     * \brief getMinAltitude
     * \return
     */
    double getMinAltitude() const;
};

#endif // __AIR_OBJ_H__

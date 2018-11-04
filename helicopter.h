#ifndef HELICOPTER_H
#define HELICOPTER_H

#include "air.h"

/*!
 * \brief The Helicopter class
 */
class Helicopter : public Air
{
protected:
    /*!
     * \brief rotationAngle
     */
    double rotationAngle = 0.0;
    /*!
     * \brief rotationStart
     */
    unsigned int rotationStart = 0;
public:
    /*!
     * \brief Helicopter constructor
     */
    Helicopter();
    /*!
     * \brief Helicopter
     * \param position
     * \param velocity
     * \param initialTime
     * \param lifeTime
     * \param acceleration
     */
    Helicopter(const Coordinates& position,
        const Vector& velocity,
        unsigned int initialTime,
        unsigned int lifeTime,
        const Vector &acceleration);
    /*!
     * \brief setRotationAngle
     * \param value
     */
    void setRotationAngle(double value);
    /*!
     * \brief getRotationAngle
     * \return
     */
    double getRotationAngle() const;
    /*!
     * \brief setRotationStart
     * \param value
     */
    void setRotationStart(unsigned int value);
    /*!
     * \brief getRotationStart
     * \return
     */
    unsigned int getRotationStart() const;
};

#endif // HELICOPTER_H

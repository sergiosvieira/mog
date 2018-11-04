#ifndef COORDINATES_H
#define COORDINATES_H

#include "vector.h"
/*!
 * \brief The Coordinates class
 */
class Coordinates
{
protected:
    /*!
     * \brief x
     */
    double x = 0.0;
    /*!
     * \brief y
     */
    double y = 0.0;
    /*!
     * \brief z
     */
    double z = 0.0;
public:
    /*!
     * \brief Coordinates constructor
     */
    Coordinates();
    /*!
     * \brief Coordinates constructor
     * \param x
     * \param y
     * \param z
     */
    Coordinates(const double& x, const double& y, const double &z);
    /*!
     * \brief setX
     * \param x
     */
    void setX(const double& x);
    /*!
     * \brief setY
     * \param y
     */
    void setY(const double& y);
    /*!
     * \brief setZ
     * \param z
     */
    void setZ(const double& z);
    /*!
     * \brief getX
     * \return
     */
    double getX() const;
    /*!
     * \brief getY
     * \return
     */
    double getY() const;
    /*!
     * \brief getZ
     * \return
     */
    double getZ() const;
    /*!
     * \brief operator +
     * \param v
     * \return
     */
    Coordinates& operator+(const Vector& v)
    {
        this->setX(this->getX() + v.getX());
        this->setY(this->getY() + v.getY());
        this->setZ((this->getZ() + v.getZ()));
        return *this;
    }
    /*!
     * \brief operator +=
     * \param p
     * \param v
     * \return
     */
    friend Coordinates& operator+=(Coordinates& p, const Vector& v)
    {
        p.x += v.getX();
        p.y += v.getY();
        p.z += v.getZ();
        return p;
    }
};

#endif // COORDINATES_H

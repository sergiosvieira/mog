#ifndef CANVAS_H
#define CANVAS_H

#include <QWidget>

#include "coordinates.h"

class TaticalMovingObject;
class QImage;

/*!
 * \brief The Canvas class
 */
class Canvas: public QWidget
{
public:
    QImage *getImage()
    {
        return this->image;
    }
protected:
    /*!
     * \brief image
     */
    QImage* image = nullptr;    
    /*!
     * \brief drawAirplane
     * \param object
     * \param position
     */
    void drawAirplane(const TaticalMovingObject& object, const Coordinates& position);
    /*!
     * \brief drawHelicopter
     * \param object
     * \param position
     */
    void drawHelicopter(const TaticalMovingObject& object, const Coordinates& position);
    /*!
     * \brief drawCar
     * \param object
     * \param position
     */
    void drawCar(const TaticalMovingObject& object, const Coordinates& position);
    /*!
     * \brief drawShip
     * \param object
     * \param position
     */
    void drawShip(const TaticalMovingObject& object, const Coordinates& position);
public:
    /*!
     * \brief Canvas constructor
     */
    Canvas();
    /*!
     * \brief Canvas constructor
     * \param parent
     */
    Canvas(QWidget* parent);
    /*!
     * \brief getCanvas
     * \return
     */
    QImage* getCanvas();
    /*!
     * \brief paintEvent
     * \param event
     */
    void paintEvent(QPaintEvent* event);
    /*!
     * \brief clear
     */
    void clear();
    /*!
     * \brief draw
     * \param object
     * \param position
     */
    void draw(const TaticalMovingObject& object, const Coordinates& position);
};

#endif // CANVAS_H

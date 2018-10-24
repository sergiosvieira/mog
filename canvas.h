#ifndef CANVAS_H
#define CANVAS_H

#include <QWidget>

#include "coordinates.h"
#include "object.h"

class QImage;

class Canvas: public QWidget
{
protected:
    QImage* image = nullptr;
    void drawAirplane(const Object& object, const Coordinates& position);
    void drawHelicopter(const Object& object, const Coordinates& position);
    void drawCar(const Object& object, const Coordinates& position);
    void drawShip(const Object& object, const Coordinates& position);
public:
    Canvas();
    Canvas(QWidget* parent);
    QImage* getCanvas();
    void paintEvent(QPaintEvent* event);
    void clear();
    void draw(const Object& object, const Coordinates& position);
};

#endif // CANVAS_H

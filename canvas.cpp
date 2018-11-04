#include "canvas.h"

#include <QPainter>
#include <cmath>
#include "objectcategory.h"
#include "taticalmovingobject.h"
#include "mapcolor.h"

void Canvas::drawAirplane(const TaticalMovingObject& object, const Coordinates& position)
{
    if (this->image)
    {
        QPainter painter(this->image);
        double x = position.getX();
        double y = position.getY();
        QBrush brush = mapTypeColor[object.getType()];
        painter.setBrush(brush);
        static const QPoint arrow[3] =
        {
            QPoint(3, 3),
            QPoint(-3, 3),
            QPoint(0, -3)
        };
        painter.translate(x, y);
        painter.drawConvexPolygon(arrow, 3);
    }
}

void Canvas::drawHelicopter(const TaticalMovingObject& object, const Coordinates& position)
{
    if (this->image)
    {
        QPainter painter(this->image);
        double x = position.getX();
        double y = position.getY();
        QBrush brush = mapTypeColor[object.getType()];
        painter.setBrush(brush);
        static const QPoint arrow[3] =
        {
            QPoint(3, 3),
            QPoint(-3, 3),
            QPoint(0, 6)
        };
        painter.translate(x, y);
        painter.drawConvexPolygon(arrow, 3);
    }
}

void Canvas::drawCar(const TaticalMovingObject& object, const Coordinates& position)
{
    if (this->image)
    {
        QPainter painter(this->image);
        double x = position.getX();
        double y = position.getY();
        QBrush brush = mapTypeColor[object.getType()];
        painter.setBrush(brush);
        QRect r(x, y, 4, 4);
        painter.drawRect(r);
    }
}

void Canvas::drawShip(const TaticalMovingObject& object, const Coordinates& position)
{
    if (this->image)
    {
        QPainter painter(this->image);
        double x = position.getX();
        double y = position.getY();
        QBrush brush = mapTypeColor[object.getType()];
        painter.setBrush(brush);
        painter.drawEllipse(x, y, 4, 4);
    }
}

Canvas::Canvas(): QWidget()
{

}

Canvas::Canvas(QWidget *parent): QWidget(parent)
{

}

QImage *Canvas::getCanvas()
{
    return this->image;
}

void Canvas::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);
    if (!this->image)
    {
        this->image = new QImage(this->size(), QImage::Format_ARGB32_Premultiplied);
        this->clear();
    }
    QPainter painter(this);
    painter.drawImage(0, 0, *this->image);
}

void Canvas::clear()
{
    if (this->image) this->image->fill(Qt::GlobalColor::white);
}

void Canvas::draw(const TaticalMovingObject& object, const Coordinates& position)
{
    if (airObjectsType.count(object.getType()) > 0)
    {
        if (object.getType() == ObjectCategory::Helicopter)
        {
            this->drawHelicopter(object, position);
        }
        else
        {
            this->drawAirplane(object, position);
        }
    }
    else if (landObjectsType.count(object.getType()) > 0)
    {
        this->drawCar(object, position);
    }
    else if (waterObjectsType.count(object.getType()) > 0
             || underwaterObjectsType.count(object.getType()) > 0)
    {
        this->drawShip(object, position);
    }
}

#include "canvas.h"

#include <QPainter>
#include <cmath>


static std::map<ObjectCategory, QBrush> mapTypeColor =
{
    {ObjectCategory::PassengerAirPlane, QBrush("#1494b8", Qt::SolidPattern)},
    {ObjectCategory::ExportationAirPlane, QBrush("#24a4b8", Qt::SolidPattern)},
    {ObjectCategory::Helicopter, QBrush("#34b4b8", Qt::SolidPattern)},
    {ObjectCategory::Missile, QBrush("#44c4b8", Qt::SolidPattern)},
    {ObjectCategory::Fighter, QBrush("#54d4b8", Qt::SolidPattern)},
    {ObjectCategory::Cargo, QBrush("#64e4b8", Qt::SolidPattern)},
    {ObjectCategory::Battletank, QBrush("#74f4b8", Qt::SolidPattern)},
    {ObjectCategory::Vehicle, QBrush("#8494b9", Qt::SolidPattern)},
    {ObjectCategory::Infantry, QBrush("#9494ba", Qt::SolidPattern)},
    {ObjectCategory::FishingShip, QBrush("#a494bb", Qt::SolidPattern)},
    {ObjectCategory::NavalShip, QBrush("#b494bc", Qt::SolidPattern)},
    {ObjectCategory::Submarine, QBrush("#c494bd", Qt::SolidPattern)},
    {ObjectCategory::UnderwaterRobot, QBrush("#d494be", Qt::SolidPattern)}
};


void Canvas::drawAirplane(const Object& object, const Coordinates& position)
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

void Canvas::drawHelicopter(const Object& object, const Coordinates& position)
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

void Canvas::drawCar(const Object& object, const Coordinates& position)
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

void Canvas::drawShip(const Object& object, const Coordinates& position)
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

void Canvas::draw(const Object& object, const Coordinates& position)
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

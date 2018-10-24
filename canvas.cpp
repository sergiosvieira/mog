#include "canvas.h"

#include <QPainter>
#include <cmath>



void Canvas::drawAirplane(const Object& object, const Coordinates& position)
{
    if (this->image)
    {
        QPainter painter(this->image);
        double x = position.getX();
        double y = position.getY();
        QBrush brush("#9494b8", Qt::SolidPattern);
        switch (object.getType())
        {
            case ObjectType::Helicopter:
                brush = QBrush("#a4a4b8", Qt::SolidPattern);
            break;
            case ObjectType::Missile:
                brush = QBrush("#b4b4b8", Qt::SolidPattern);
            break;
            case ObjectType::Cargo:
                brush = QBrush("#c4c4b8", Qt::SolidPattern);
            break;
            case ObjectType::Boing:
                brush = QBrush("#d4d4b8", Qt::SolidPattern);
            break;
            case ObjectType::Fighter:
                brush = QBrush("#e4e4b8", Qt::SolidPattern);
            break;

        }
        painter.setBrush(brush);
        //painter.drawEllipse(x, y, 4, 4);
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
        painter.setBrush(QBrush("#66ccff", Qt::SolidPattern));
        painter.drawEllipse(x, y, 4, 4);
    }
}

void Canvas::drawCar(const Object& object, const Coordinates& position)
{
    if (this->image)
    {
        QPainter painter(this->image);
        double x = position.getX();
        double y = position.getY();
        painter.setBrush(QBrush("#ff330", Qt::SolidPattern));
        painter.drawEllipse(x, y, 4, 4);
    }
}

void Canvas::drawShip(const Object& object, const Coordinates& position)
{
    if (this->image)
    {
        QPainter painter(this->image);
        double x = position.getX();
        double y = position.getY();
        painter.setBrush(QBrush("#009999", Qt::SolidPattern));
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
    switch (object.getType())
    {
    case ObjectType::AirPlane:
    case ObjectType::Missile:
    case ObjectType::Cargo:
    case ObjectType::Boing:
    case ObjectType::Fighter:
        this->drawAirplane(object, position);
        break;
    case ObjectType::Helicopter:
        this->drawHelicopter(object, position);
        break;
    case ObjectType::Land:
        this->drawCar(object, position);
        break;
    case ObjectType::OnWater:
    case ObjectType::Underwater:
        this->drawShip(object, position);
        break;
    }
}

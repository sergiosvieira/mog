#ifndef CG_H
#define CG_H

#include <QRectF>
#include <QPointF>
#include "coordinates.h"


namespace CG
{
    static QRectF World{0, 0, 576000000., 576000000.};
    static QRectF Window = World;
    static QRectF ViewPort{0., 0., 640., 480.};
    static QPointF w2vp(Coordinates &p)
    {
        QPointF result;
        double sx = (ViewPort.width() - ViewPort.left()) / (World.width() - World.left());
        double sy = (ViewPort.height() - ViewPort.top()) / (World.height() - World.top());;
        double x = sx * (p.getX() - World.left()) + ViewPort.left();
        double y = sy * (p.getY() - World.top()) + ViewPort.top();
        result.setX(x);
        result.setY(y);
        return result;
    }
}

#endif // CG_H

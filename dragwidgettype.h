#ifndef DRAGWIDGETTYPE_H
#define DRAGWIDGETTYPE_H

#include <QPointF>

/*!
 * \brief The DragWidgetType enum
 */
enum class DragWidgetType
{
    ShipNK,
    ShipSK,
    AirportNK,
    AirportSK,
    HelicopterCarNK,
    HelicopterCarSK
};

/*!
 * \brief The Station struct
 */
struct Station
{
    DragWidgetType type;
    QPointF pos;
};

#endif // DRAGWIDGETTYPE_H

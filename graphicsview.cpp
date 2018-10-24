#include "graphicsview.h"
#include <QDragMoveEvent>
#include <QMimeData>
#include <mainwindow.h>
#include <QBrush>
#include <QPen>
#include <QFont>

GraphicsView::GraphicsView(GraphicsViewType viewType, MainWindow *parent)
{
    type = viewType;
    mainWindow = parent;
}

// matrix coordination system
void GraphicsView::drawWhiteArea()
{
    double width = this->width();
    double height = this->height();
    this->setSceneRect(0, 0, width, height);

    double x = width / 4.0;
    double y = (type == GraphicsViewType::NK)?height / 4.0:0.0;
    double w = width / 2.0 + 8.0;
    double h = height * 3.0 / 4.0 + 8.0;
    this->scene()->addRect(x, y, w, h, QPen(Qt::white), QBrush(Qt::white));

    whiteArea.setX(x);
    whiteArea.setY(y);
    whiteArea.setWidth(w);
    whiteArea.setHeight(h);
}

// opengl coordination system
void GraphicsView::getWhiteArea(const QRectF &world, QRectF &out)
{
    out.setX(world.width() / 4.0);
    out.setY(world.height() / 8.0);
    out.setWidth(world.width() / 2.0);
    out.setHeight(world.height() * 3.0 / 4.0);
}

void GraphicsView::dragEnterEvent(QDragEnterEvent *event)
{
    bool detail;
    DragWidgetType objType;
    if (isAvailable(event->mimeData()->text(), event->posF(), objType, detail)) {
        if (detail) {
            event->accept();
        } else {
            event->accept(whiteArea.toRect());
        }
    } else {
        event->ignore();
    }
}

void GraphicsView::dragMoveEvent(QDragMoveEvent *event)
{
    bool detail;
    DragWidgetType objType;
    if (isAvailable(event->mimeData()->text(), event->posF(), objType, detail)) {
        if (detail) {
            event->accept();
        } else {
            event->accept(whiteArea.toRect());
        }
    } else {
        event->ignore();
    }
}

void GraphicsView::dropEvent(QDropEvent *event)
{
    bool detail;
    DragWidgetType objType;
    QString text = event->mimeData()->text();
    if (isAvailable(text, event->posF(), objType, detail)) {
        if (!detail) {
            event->ignore();
            return;
        }
        event->accept();

        QGraphicsPixmapItem *item = new QGraphicsPixmapItem(QPixmap(":/images/" + text + ".png"));
        item->setPos(event->pos());
        this->scene()->addItem(item);

        int no = 0;
        for (const auto s: vecStation) {
            if (s.type == objType) {
                no++;
            }
        }
        no++;
        Station station;
        station.type = objType;
        station.pos = event->pos();
        vecStation.emplace_back(station);
        QGraphicsTextItem *txt = new QGraphicsTextItem(QString::number(no), item);
        txt->setPos(4.0, 1.0);
        txt->setFont(QFont("Times", 15, QFont::Bold));
        this->scene()->addItem(txt);
    } else {
        event->ignore();
    }
}

void GraphicsView::addStation(const Station &s)
{
     vecStation.emplace_back(s);
}

void GraphicsView::getAllStation(std::vector<Station> &out)
{
    for (auto s : vecStation) {
        out.emplace_back(s);
    }
}

void GraphicsView::clear()
{
    vecStation.clear();
}

bool GraphicsView::isAvailable(const QString &text, const QPointF &position, DragWidgetType &objType, bool &detail)
{
    if (text == "ShipNK") {
        objType = DragWidgetType::ShipNK;
    } else if (text == "ShipSK") {
        objType = DragWidgetType::ShipSK;
    } else if (text == "AirportNK") {
        objType = DragWidgetType::AirportNK;
    } else if (text == "AirportSK") {
        objType = DragWidgetType::AirportSK;
    } else if (text == "HelicopterCarNK") {
        objType = DragWidgetType::HelicopterCarNK;
    } else if (text == "HelicopterCarSK") {
        objType = DragWidgetType::HelicopterCarSK;
    } else {
        return false;
    }
    if (mainWindow->getObjectCount() == 0) {
        return false;
    }
    if (text.endsWith("NK") && type != GraphicsViewType::NK) {
        return false;
    }
    if (text.endsWith("SK") && type != GraphicsViewType::SK) {
        return false;
    }
    if (!text.startsWith("Ship") && !text.startsWith("Airport") && !text.startsWith("HelicopterCar")) {
        return false;
    }
    bool containsWhitearea = whiteArea.contains(position);
    switch (objType) {
    case DragWidgetType::ShipNK:
    case DragWidgetType::ShipSK:
        detail = !containsWhitearea;
        break;
    default:
        detail = containsWhitearea;
        break;
    }
    return true;
}

GraphicsViewType GraphicsView::getType()
{
    return type;
}

#include <QtWidgets>

#include "dragwidget.h"

DragWidget::DragWidget(DragWidgetType dragType, QWidget *parent)
    : QLabel(parent)
{
    setMinimumSize(32, 32);
    setFrameStyle(QFrame::Sunken | QFrame::StyledPanel);
//    setAcceptDrops(true);

    QString imgPath;
    switch (dragType) {
    case DragWidgetType::ShipNK:
        imgPath = ":/images/ShipNK.png";
        break;
    case DragWidgetType::ShipSK:
        imgPath = ":/images/ShipSK.png";
        break;
    case DragWidgetType::AirportNK:
        imgPath = ":/images/AirportNK.png";
        break;
    case DragWidgetType::AirportSK:
        imgPath = ":/images/AirportSK.png";
        break;
    case DragWidgetType::HelicopterCarNK:
        imgPath = ":/images/HelicopterCarNK";
        break;
    case DragWidgetType::HelicopterCarSK:
        imgPath = ":/images/HelicopterCarSK.png";
        break;
    }
    type = dragType;
    setPixmap(QPixmap(imgPath));
    move(0, 0);
    setAttribute(Qt::WA_DeleteOnClose);
}

void DragWidget::mousePressEvent(QMouseEvent *event)
{
    QPixmap pixmap = *this->pixmap();;

    QMimeData *mimeData = new QMimeData;
    switch (type) {
    case DragWidgetType::ShipNK:
        mimeData->setText("ShipNK");
        break;
    case DragWidgetType::ShipSK:
        mimeData->setText("ShipSK");
        break;
    case DragWidgetType::AirportNK:
        mimeData->setText("AirportNK");
        break;
    case DragWidgetType::AirportSK:
        mimeData->setText("AirportSK");
        break;
    case DragWidgetType::HelicopterCarNK:
        mimeData->setText("HelicopterCarNK");
        break;
    case DragWidgetType::HelicopterCarSK:
        mimeData->setText("HelicopterCarSK");
        break;
    }

    QDrag *drag = new QDrag(this);
    drag->setMimeData(mimeData);
    drag->setPixmap(pixmap);
    drag->setHotSpot(event->pos());

    QPixmap tempPixmap = pixmap;
    QPainter painter;
    painter.begin(&tempPixmap);
    painter.fillRect(pixmap.rect(), QColor(127, 127, 127, 127));
    painter.end();

    setPixmap(tempPixmap);

    if (drag->exec(Qt::CopyAction | Qt::MoveAction, Qt::CopyAction) == Qt::MoveAction) {
        close();
    } else {
        show();
        setPixmap(pixmap);
    }
}

DragWidgetType DragWidget::getType()
{
    return type;
}

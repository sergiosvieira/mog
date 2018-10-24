#ifndef DRAGWIDGET_H
#define DRAGWIDGET_H

#include <QLabel>
#include "object.h"

class QDragEnterEvent;
class QDropEvent;

class DragWidget : public QLabel
{
public:
    explicit DragWidget(DragWidgetType dragType, QWidget *parent = nullptr);
    DragWidgetType getType();
protected:
    void mousePressEvent(QMouseEvent *event) override;
private:
    DragWidgetType type;
};

#endif

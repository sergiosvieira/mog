#ifndef GRAPHICSVIEW_H
#define GRAPHICSVIEW_H

#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include "object.h"

class QDragEnterEvent;
class QDropEvent;
class MainWindow;
class GraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit GraphicsView(GraphicsViewType viewType, MainWindow *parent);
    void getAllStation(std::vector<Station> &out);
    void drawWhiteArea();
    static void getWhiteArea(const QRectF &world, QRectF &out);
    GraphicsViewType getType();
    void addStation(const Station &s);
    void clear();
protected:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dragMoveEvent(QDragMoveEvent *event) override;
    void dropEvent(QDropEvent *event) override;

private:
    GraphicsViewType type;
    MainWindow *mainWindow;
    std::vector<Station> vecStation;
    QRectF whiteArea;

private:
    bool isAvailable(const QString &text, const QPointF &position, DragWidgetType &objType, bool &detail);
};

#endif // GRAPHICSVIEW_H

#ifndef GRAPHICSVIEW_H
#define GRAPHICSVIEW_H

#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include "dragwidgettype.h"
#include "graphicsviewtype.h"

class TaticalMovingObject;
class QDragEnterEvent;
class QDropEvent;
class MainWindow;

/*!
 * \brief The GraphicsView class
 */
class GraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    /*!
     * \brief GraphicsView
     * \param viewType
     * \param parent
     */
    explicit GraphicsView(GraphicsViewType viewType, MainWindow *parent);
    /*!
     * \brief getAllStation
     * \param out
     */
    void getAllStation(std::vector<Station> &out);
    /*!
     * \brief drawWhiteArea
     */
    void drawWhiteArea();
    /*!
     * \brief getWhiteArea
     * \param world
     * \param out
     */
    static void getWhiteArea(const QRectF &world, QRectF &out);
    /*!
     * \brief getType
     * \return
     */
    GraphicsViewType getType();
    /*!
     * \brief addStation
     * \param s
     */
    void addStation(const Station &s);
    /*!
     * \brief clear
     */
    void clear();
protected:
    /*!
     * \brief dragEnterEvent
     * \param event
     */
    void dragEnterEvent(QDragEnterEvent *event) override;
    /*!
     * \brief dragMoveEvent
     * \param event
     */
    void dragMoveEvent(QDragMoveEvent *event) override;
    /*!
     * \brief dropEvent
     * \param event
     */
    void dropEvent(QDropEvent *event) override;

private:
    /*!
     * \brief type
     */
    GraphicsViewType type;
    /*!
     * \brief mainWindow
     */
    MainWindow *mainWindow;
    /*!
     * \brief vecStation
     */
    std::vector<Station> vecStation;
    /*!
     * \brief whiteArea
     */
    QRectF whiteArea;

private:
    /*!
     * \brief isAvailable
     * \param text
     * \param position
     * \param objType
     * \param detail
     * \return
     */
    bool isAvailable(const QString &text, const QPointF &position, DragWidgetType &objType, bool &detail);
};

#endif // GRAPHICSVIEW_H

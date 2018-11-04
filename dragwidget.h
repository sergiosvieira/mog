#ifndef DRAGWIDGET_H
#define DRAGWIDGET_H

#include <QLabel>
#include "dragwidgettype.h"

class TaticalMovingObject;
class QDragEnterEvent;
class QDropEvent;

/*!
 * \brief The DragWidget class
 */
class DragWidget : public QLabel
{
public:
    /*!
     * \brief DragWidget
     * \param dragType
     * \param parent
     */
    explicit DragWidget(DragWidgetType dragType, QWidget *parent = nullptr);
    /*!
     * \brief getType
     * \return
     */
    DragWidgetType getType();
protected:
    /*!
     * \brief mousePressEvent
     * \param event
     */
    void mousePressEvent(QMouseEvent *event) override;
private:
    /*!
     * \brief type
     */
    DragWidgetType type;
};

#endif

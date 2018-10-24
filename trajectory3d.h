#ifndef TRAJECTORY3D_H
#define TRAJECTORY3D_H

#include <QDialog>
#include <QtDataVisualization/q3dscatter.h>
#include <QtDataVisualization/qabstract3dseries.h>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>

using namespace QtDataVisualization;
QT_CHARTS_USE_NAMESPACE

namespace Ui {
class Trajectory3D;
}

class Trajectory3D : public QDialog
{
    Q_OBJECT

public:
    explicit Trajectory3D(QWidget *parent = nullptr);
    ~Trajectory3D();

    void drawData3D(const std::vector<QVector3D> &data);
    void drawData2D(const std::vector<QPointF> &data);
private:
    Ui::Trajectory3D *ui;
};

#endif // TRAJECTORY3D_H

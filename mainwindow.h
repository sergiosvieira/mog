#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <QMainWindow>
#include <QTimer>
#include <vector>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include "object.h"

static const QRect World{0, 0, 192000, 192000};


#include "datamanager.h"

class Canvas;
class QTableView;

QT_CHARTS_USE_NAMESPACE

namespace Ui {
class MainWindow;
}


using VectorOfObject = std::vector<Object*>;

class MainWindow : public QMainWindow
{
    Q_OBJECT
protected:
    QTimer* timer = nullptr;
    Canvas* airCanvas = nullptr;
    DataManager dataManager;
    VectorOfObject objects;
    QChart *chart;
    QLineSeries *series;
    QChartView *chartView;
    QTableView *positionTable = nullptr;
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void paintEvent(QPaintEvent* event);
    double getMaxVelocity(const ObjectType &type);
    double getMinVelocity(const ObjectType &type);
    double getMaxAcceleration(const ObjectType &type);
    double getMinAcceleration(const ObjectType &type);
    double getMaxAltitude(const ObjectType &type);
    double getMinAltitude(const ObjectType &type);
    double getMaxDepth(const ObjectType &type);
    double getMinDepth(const ObjectType &type);
    unsigned int getInitialTime();
    unsigned int getLifeTime();
private slots:
    void on_btn_air_generate_clicked();

    void on_timeSlider_valueChanged(int value);

    void on_AddPattern_clicked();

    void on_objectsID_activated(const QString &arg1);

    void on_objectsID_activated(int index);

    void on_pushButton_clicked();

    void keyPressEvent(QKeyEvent *event);
private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H

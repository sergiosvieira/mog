#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <QMainWindow>
#include <QTimer>
#include <vector>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>

static const QRect World{0, 0, 192000, 192000};


#include "datamanager.h"

class Canvas;

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
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void paintEvent(QPaintEvent* event);
    void adjustAll();
private slots:
    void on_btn_air_generate_clicked();

    void on_timeSlider_valueChanged(int value);

    void on_AddPattern_clicked();

    void on_objectsID_activated(const QString &arg1);

    void on_objectsID_activated(int index);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H

#ifndef HISTOGRAM_H
#define HISTOGRAM_H

#include <QDialog>
#include <QtCharts/QChartView>

namespace Ui {
class Histogram;
}

QT_CHARTS_USE_NAMESPACE

class MainWindow;
class Histogram : public QDialog
{
    Q_OBJECT

public:
    explicit Histogram(QWidget *parent = nullptr);
    ~Histogram();

    void setData(const std::array<std::vector<double>, 24> &data, double min, double max);
    void updateGraph(int timeIndex);
private slots:
    void onTimeSelected(int timeIndex); // timeIndex is 0-base
private:
    Ui::Histogram *ui;

    MainWindow *parentWindow;
    QChart *chart;
    QChartView *graph;

    bool initialized;
    double minValue, maxValue;
    //std::array<std::vector<double>, 24> graphData;


    void closeEvent(QCloseEvent *e);
};

#endif // HISTOGRAM_H

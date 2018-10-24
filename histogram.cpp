#include "histogram.h"
#include "ui_histogram.h"
#include <QMessageBox>
#include <QBarSet>
#include <QBarSeries>
#include <QBarCategoryAxis>
#include <QValueAxis>
#include "mainwindow.h"
#include <float.h>
#include <QApplication>
#include <QDesktopWidget>

Histogram::Histogram(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Histogram),
    initialized(false)
{
    ui->setupUi(this);

    parentWindow = static_cast<MainWindow *>(parent);
    for (int i = 0; i < 24; i++)
    {
        ui->cmbTime->addItem(QString::number(i));
    }

    this->chart = new QChart();
    this->graph = new QChartView(chart, this);
    ui->mainLayout->addWidget(this->graph);

    this->setGeometry(0, 200, QApplication::desktop()->screenGeometry().width(), 700);
}

Histogram::~Histogram()
{
    delete ui;
}

void Histogram::closeEvent(QCloseEvent *e)
{
//    initialized = false;
//    for (int i = 0; i < 24; i ++) {
//        graphData[i].clear();
//    }
}

void Histogram::setData(const std::array<std::vector<double>, 24> &data, double min, double max)
{
//    minValue = min;
//    maxValue = max;
//    for (int i = 0; i < 24; i ++) {
//        graphData[i].clear();
//        for (const auto v : data[i]) {
//            graphData[i].push_back(v);
//        }
//    }
//    initialized = true;
//    updateGraph(0);
}

void Histogram::onTimeSelected(int timeIndex)
{
    if (!initialized) {
        return;
    }
    chart->removeAxis(chart->axisX());
    chart->removeAxis(chart->axisY());
    chart->removeAllSeries();
    updateGraph(timeIndex);
}

void Histogram::updateGraph(int timeIndex)
{
//    if (!initialized) {
//        return;
//    }
//    std::vector<double> data = graphData[timeIndex];

//    // 1. sorting
//    std::sort (data.begin(), data.end(), [](float a, float b)->bool {
//        return (std::abs(a) < std::abs(b));
//    });
//    // 2. determine the count of group
//    int nGroup;
//    int count = data.size();
//    if (count > 5000) {
//        nGroup = 50;
//    } else if (count > 1000) {
//        nGroup = 20;
//    } else if (count > 100) {
//        nGroup = 15;
//    } else if (count > 10) {
//        nGroup = 10;
//    } else {
//        QMessageBox messageBox;
//        messageBox.critical(0,"Error","Your data count is very less.");
//        messageBox.setFixedSize(500,200);
//        return;
//    }
//    // 3. determine y-value and make x-label
//    std::vector<QString> vecXLabel;
//    std::vector<int> vecYValue;
//    double step = (maxValue - minValue) / nGroup;
//    int currentIndex = 0;
//    int maxYValue = 0;
//    for (int i = 0; i < nGroup; i++) {
//        double lbound = i * step;
//        double ubound = (i + 1) * step;
//        int yValue = 0;
//        for (int j = currentIndex; j < count; j++) {
//            double value = data[j];
//            if (value >= lbound && value < ubound) {
//                yValue++;
//                currentIndex++;
//            } else {
//                break;
//            }
//        }
//        if (maxYValue < yValue) {
//            maxYValue = yValue;
//        }
//        vecXLabel.push_back(QString::number(floor(lbound)) + "-" + QString::number(floor(ubound)));
//        vecYValue.push_back(yValue);
//    }
//    // 4. draw
//    QStringList categories;
//    QBarSeries *barseries = new QBarSeries();
//    QBarSet *set = new QBarSet("Velocity Histogram");
//    for (int i = 0; i < nGroup; i++) {
//        *set << vecYValue[i];
//        categories << vecXLabel[i];
//    }
//    barseries->append(set);
//    chart->addSeries(barseries);

//    QBarCategoryAxis *axisX = new QBarCategoryAxis();
//    axisX->setCategories(categories);
//    chart->setAxisX(axisX, barseries);
//    axisX->setRange(categories[0], categories[nGroup - 1]);

//    QValueAxis *axisY = new QValueAxis();
//    chart->setAxisY(axisY, barseries);
//    axisY->setRange(0, maxYValue);

//    chart->legend()->setVisible(true);
//    chart->legend()->setAlignment(Qt::AlignBottom);

//    graph->setRenderHint(QPainter::Antialiasing);
}

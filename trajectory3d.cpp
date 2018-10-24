#include "trajectory3d.h"
#include "ui_trajectory3d.h"
#include <QMessageBox>

Trajectory3D::Trajectory3D(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Trajectory3D)
{
    ui->setupUi(this);
}

Trajectory3D::~Trajectory3D()
{
    delete ui;
}

void Trajectory3D::drawData3D(const std::vector<QVector3D> &data)
{
    Q3DScatter *graph = new Q3DScatter();
    graph->activeTheme()->setType(Q3DTheme::ThemeQt);
    QFont font = graph->activeTheme()->font();
    font.setPointSize(40.0f);
    graph->activeTheme()->setFont(font);
    graph->setShadowQuality(QAbstract3DGraph::ShadowQualitySoftLow);
    graph->scene()->activeCamera()->setCameraPreset(Q3DCamera::CameraPresetFront);

    QScatterDataProxy *proxy = new QScatterDataProxy;
    QScatter3DSeries *series = new QScatter3DSeries(proxy);
    series->setItemLabelFormat(QStringLiteral("@xTitle: @xLabel @yTitle: @yLabel @zTitle: @zLabel"));
    series->setMeshSmooth(true);
    graph->addSeries(series);

    graph->axisX()->setTitle("X");
    graph->axisY()->setTitle("Y");
    graph->axisZ()->setTitle("Z");

    QWidget *container = QWidget::createWindowContainer(graph);
    ui->mainLayout->addWidget(container);

    int count = data.size();

    QScatterDataArray *dataArray = new QScatterDataArray;
    dataArray->resize(count);
    QScatterDataItem *ptrToDataArray = &dataArray->first();
    for (const auto pos : data) {
        ptrToDataArray->setPosition(pos);
        ptrToDataArray++;
    }
    graph->seriesList().at(0)->dataProxy()->resetArray(dataArray);
}

void Trajectory3D::drawData2D(const std::vector<QPointF> &data)
{
     QLineSeries *series = new QLineSeries();

     for (const auto pos : data) {
         series->append(pos);
     }

     QChart *chart = new QChart();
     chart->legend()->hide();
     chart->addSeries(series);
     chart->createDefaultAxes();
     chart->setTitle("2D trajectory");

     QChartView *chartView = new QChartView(chart);
     chartView->setRenderHint(QPainter::Antialiasing);

     ui->mainLayout->addWidget(chartView);
}

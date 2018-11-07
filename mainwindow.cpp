#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QListWidgetItem>
#include <QTableView>
#include <QStandardItemModel>
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtCharts/QValueAxis>
#include <QtWidgets>
#include <QMessageBox>
#include <QFile>
#include <QSet>
#include <random>
#include "canvas.h"
#include "airplane.h"
#include "land.h"
#include "helicopter.h"
#include "water.h"
#include "objectgenerator.h"

#include <float.h>
#include <memory>
#include "histogram.h"
#include "trajectory3d.h"
#include <QGraphicsPixmapItem>
#include <set>
#include "stationsetting.h"
#include "objectfactory.h"
#include "collectedeach.h"
#include "airobjectsmanager.h"

Coordinates transform
(
    const Coordinates& position,
    const QRect& srcRect,
    const QRect& dstRect
)
{
    double rx = (position.getX() - srcRect.topLeft().x()) / srcRect.width();
    double ry = (position.getY() - srcRect.topLeft().y()) / srcRect.height();
    Coordinates result
    (
        dstRect.topLeft().x() + dstRect.width() * rx,
        dstRect.topLeft().y() + dstRect.height() * ry,
        0.0
    );
    return result;
}

double kmh2ms(double value)
{
    return value * 0.277778;
}

double ms2kmh(double value)
{
    return value / 0.277778;
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    airCanvas = new Canvas(this);
    airCanvas->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    ui->air_hlayout->addWidget(airCanvas);

    this->chart = new QChart();
    QValueAxis* axisX = new QValueAxis();
    axisX->setRange(0, 23);
    axisX->setTickCount(1);
    this->series = new QLineSeries();
    this->chartView = new QChartView(this->chart);
    this->chartView->chart()->setAxisX(axisX);
    ui->trajectoryHLayout->addWidget(this->chartView);

    this->positionTable = new QTableView(this);
    QStandardItemModel* posModel = new QStandardItemModel(this);
    posModel->insertColumn(0, QModelIndex());
    posModel->insertColumn(1, QModelIndex());
    posModel->insertColumn(2, QModelIndex());
    posModel->insertColumn(3, QModelIndex());
    posModel->insertColumn(4, QModelIndex());
    posModel->setHeaderData(0, Qt::Horizontal, QObject::tr("Time"));
    posModel->setHeaderData(1, Qt::Horizontal, QObject::tr("Position X"));
    posModel->setHeaderData(2, Qt::Horizontal, QObject::tr("Position Y"));
    posModel->setHeaderData(3, Qt::Horizontal, QObject::tr("Position Z"));
    posModel->setHeaderData(4, Qt::Horizontal, QObject::tr("Movement Pattern"));
    this->positionTable->setModel(posModel);

    ui->tab_2->layout()->addWidget(this->positionTable);
    initPatternTable();
    initObjectsTable();
    initStationTab();

}

void MainWindow::initKalmanFilter()
{
    int n = 3; // Number of states
    int m = 1; // Number of measurements
    double dt = 1.0/30; // Time step
    Eigen::MatrixXd A(n, n); // System dynamics matrix
    Eigen::MatrixXd C(m, n); // Output matrix
    Eigen::MatrixXd Q(n, n); // Process noise covariance
    Eigen::MatrixXd R(m, m); // Measurement noise covariance
    Eigen::MatrixXd P(n, n); // Estimate error covariance
    // Discrete LTI projectile motion, measuring position only
    A << 1, dt, 0, 0, 1, dt, 0, 0, 1;
    C << 1, 0, 0;
    // Reasonable covariance matrices
    Q << .05, .05, .0, .05, .05, .0, .0, .0, .0;
    R << 5;
    P << .1, .1, .1, .1, 10000, 10, .1, 10, 100;
}

void MainWindow::initPositionTable()
{
    if (this->positionTable == nullptr)
    {
        this->positionTable = new QTableView(this);
    }
    QStandardItemModel *posModel = (QStandardItemModel*)this->positionTable->model();
    if (!posModel) posModel = new QStandardItemModel(this);
    posModel->insertColumn(0, QModelIndex());
    posModel->insertColumn(1, QModelIndex());
    posModel->insertColumn(2, QModelIndex());
    posModel->insertColumn(3, QModelIndex());
    posModel->insertColumn(4, QModelIndex());
    posModel->setHeaderData(0, Qt::Horizontal, QObject::tr("Time"));
    posModel->setHeaderData(1, Qt::Horizontal, QObject::tr("Position X"));
    posModel->setHeaderData(2, Qt::Horizontal, QObject::tr("Position Y"));
    posModel->setHeaderData(3, Qt::Horizontal, QObject::tr("Position Z"));
    posModel->setHeaderData(4, Qt::Horizontal, QObject::tr("Movement Pattern"));
    this->positionTable->setModel(posModel);
}

void MainWindow::initPatternTable()
{
    QAbstractItemModel* model = this->ui->patternTable->model();
    if (model == nullptr)
    {
        model = new QStandardItemModel(this);
        this->ui->patternTable->setModel(model);
    }
    model->insertColumn(0, QModelIndex());
    model->insertColumn(1, QModelIndex());
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("Pattern"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Time"));
}

void MainWindow::initObjectsTable()
{
    QAbstractItemModel* model = this->ui->tableObjects->model();
    if (model == nullptr)
    {
        model = new QStandardItemModel(this);
        this->ui->tableObjects->setModel(model);
    }
    QVector<QString> titles =
    {
        QObject::tr("Type"),
        QObject::tr("Total"),
        QObject::tr("Max. Velocity"),
        QObject::tr("Min. Velocity"),
        QObject::tr("Max. Acceleration"),
        QObject::tr("Min. Acceleration"),
        QObject::tr("Max. Altitude"),
        QObject::tr("Min. Altitude"),
        QObject::tr("Rotation Angle (RA)"),
        QObject::tr("RA Starting Time"),
        QObject::tr("Max. Depth"),
        QObject::tr("Min. Depth"),
        QObject::tr("Collected Each")
    };
    for (int i = 0; i < titles.size(); ++i)
    {
        model->insertColumn(i, QModelIndex());
        model->setHeaderData(i, Qt::Horizontal, titles[i]);
    }
}

MainWindow::~MainWindow()
{
//    for (std::map<unsigned int, TaticalMovingObject*>::iterator it = this->objects.begin();
//         it != this->objects.end(); ++it)
//    {
//        if ((*it).second != nullptr) delete (*it).second;
//        this->objects.erase(it);
//    }
    delete ui;

    delete sceneNK;
    delete sceneSK;

    delete shipNK;
    delete shipSK;
    delete airPortNK;
    delete airPortSK;
    delete helicopterCarNK;
    delete helicopterCarSK;
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    int time = this->ui->timeSlider->value();
    std::vector<TaticalMovingObjectsManager::Map&> v = {
        this->airManager.getObjects(),
        this->landManager.getObjects(),
        this->waterManager.getObjects()
    };
    for (auto m: v)
    {
        for (auto object: v)
        {
            unsigned int id = object.second->getID();
            Pair p = dataManager.getData(time, id);
            Coordinates position = p.first;
            Coordinates u = transform(position, World, this->airCanvas->rect());
            this->airManager.show(id, this->airCanvas->getImage());
        }
    }
}

void MainWindow::onHistogramClicked()
{
//    int count = this->objects.size();
//    if (count == 0) {
//        return;
//    }
//    std::vector<std::vector<double>> arrVelocity(this->lifetimeValue);
//    double min = DBL_MAX, max = 0.0;
//    for (int t = 0; t < 24; t ++) {
//        for (auto o : this->objects) {
//            Pair p = this->dataManager.getData(t, o->getID());
//            Vector vel = p.second;
//            double magnitude = vel.length();
//            if (magnitude >= max) {
//                max = magnitude;
//            }
//            if (magnitude < min) {
//                min = magnitude;
//            }
//            arrVelocity[t].push_back(magnitude);
//        }
//    }
//    Histogram *histogramWindow = new Histogram(this);
//    histogramWindow->setData(arrVelocity, min, max);
//    histogramWindow->show();
}

void MainWindow::onTrajectoryClicked()
{
//    if (this->objects.size() == 0) {
//        return;
//    }
    if (this->airManager.getObjects().size() == 0
            || this->landManager.getObjects().size() == 0
            || this->waterManager.getObjects().size() == 0)
    {
        return;
    }

    TaticalMovingObject *obj = this->objectMap[ui->objectsID->currentText()];
    if (!obj) return;
    ObjectCategory objType = obj->getType();
    Trajectory3D *trajectoryWindow = new Trajectory3D(this);
    trajectoryWindow->show();

    std::set<ObjectCategory> noZ = {
        ObjectCategory::Battletank,
        ObjectCategory::Vehicle,
        ObjectCategory::Infantry,
        ObjectCategory::FishingShip,
        ObjectCategory::NavalShip
    };

    if (noZ.count(objType) > 0)
    {
        std::vector<QPointF> data;
        for (int t = 0; t < this->lifetimeValue; ++t)
        {
            Pair p = this->dataManager.getData(t, obj->getID());
            Coordinates pos = p.first;
            data.emplace_back(QPointF(pos.getX(), pos.getY()));
        }
        trajectoryWindow->drawData2D(data);
    } else {
        std::vector<QVector3D> data;
        for (int t = 0; t < this->lifetimeValue; ++t)
        {
            Pair p = this->dataManager.getData(t, obj->getID());
            Coordinates pos = p.first;
            data.emplace_back(QVector3D(pos.getX(), pos.getY(), pos.getZ()));
        }
        trajectoryWindow->drawData3D(data);
    }
}

int MainWindow::tableObjectRowCount()
{
    int result = this->ui->tableObjects->model()->rowCount();
    return result;
}

QString MainWindow::getFirstValidObject()
{
    QString result = "AirPlane";
    for (auto kv: this->airManager.getObjects())
    {
        auto object = kv.second;
        return QString::fromStdString(TaticalMovingObject::stringFromType(object->getType()));
    }
    for (auto kv: this->landManager.getObjects())
    {
        auto object = kv.second;
        return QString::fromStdString(TaticalMovingObject::stringFromType(object->getType()));
    }
    for (auto kv: this->waterManager.getObjects())
    {
        auto object = kv.second;
        return QString::fromStdString(TaticalMovingObject::stringFromType(object->getType()));
    }
    return result;
}

void MainWindow::updateObjectList(const QString& str)
{
    this->ui->objectsID->clear();
    ObjectCategory type = TaticalMovingObject::typeFromString(str.toStdString());    
    bool flag = true;
    for (auto m: {this->airManager.getObjects(), this->landManager.getObjects(), this->waterManager.getObjects()})
    for (auto kv: m)
    {
        auto object = kv.second;
        if (object->getType() != type) continue;
        if (flag)
        {
            flag = false;
            int index = this->ui->objectCategories->findText(str);
            this->ui->objectCategories->setCurrentIndex(index);
        }
        this->ui->objectsID->addItem(object->getName());
        this->objectMap[object->getName()] = object;
    }
}

void MainWindow::configurePattern(TaticalMovingObject *object)
{
    unsigned int instant = 0;
    QStandardItemModel* model = (QStandardItemModel*)this->ui->patternTable->model();
    for (int i = 0; i < model->rowCount(); ++i)
    {
        QString patternStr = model->data(model->index(i, 0)).toString();
        QString instantStr = model->data(model->index(i, 1)).toString();
        instant = instantStr.toInt();
        MovingPattern pattern = Pattern::patternFromString(patternStr.toStdString());
        object->addPattern(pattern, instant);
    }
}

DistributionType MainWindow::distributionFromIndex(int index)
{
    DistributionType distributionType = DistributionType::Uniform;
    switch(index)
    {
    case 0:
        break;
    case 1:
        distributionType = DistributionType::Poisson;
        break;
    case 2:
        distributionType = DistributionType::Gaussian;
        break;
    case 3:
        distributionType = DistributionType::LeftSkewed;
        break;
    case 4:
        distributionType = DistributionType::RightSkewed;
        break;
    default:
        QMessageBox messageBox;
        messageBox.critical(0,"Error","Invalid Distribution Type!");
        messageBox.setFixedSize(500,200);
    }
    return distributionType;
}

void MainWindow::clearObjects()
{
    for (auto m: {this->airManager.getObjects(), this->landManager.getObjects(), this->waterManager.getObjects()})
    for (std::map<unsigned int, TaticalMovingObject*>::iterator it = m.begin();
         it != m.end(); ++it)
    {
        if ((*it).second != nullptr) delete (*it).second;
        m.erase(it);
    }
}

bool MainWindow::checkObjects(int n)
{
    if (n == 0)
    {
        QMessageBox messageBox;
        messageBox.critical(0,"Error","You must adds objects first");
        messageBox.setFixedSize(500,200);
        return false;
    }
    return true;
}

bool MainWindow::checkPatterns()
{
    if (ui->patternTable->model()->rowCount() == 0)
    {
        QMessageBox messageBox;
        messageBox.critical(0,"Error","You must adds at least one moving pattern");
        messageBox.setFixedSize(500,200);
        return false;
    }
    return true;
}

void MainWindow::updateAirObjects()
{
    for (auto kv: this->airManager.getObjects())
    {
        this->airManager.update(kv.first);
    }
}

void MainWindow::updateLandObjects()
{
    for (auto kv: this->landManager.getObjects())
    {
        this->landManager.update(kv.first);
    }
}

void MainWindow::updateWaterObjects()
{
    for (auto kv: this->waterManager.getObjects())
    {
        this->waterManager.update(kv.first);
    }
}

void MainWindow::on_btn_air_generate_clicked()
{
    this->lifetimeValue = this->ui->lifeTime->value();
    unsigned int initialTime = this->ui->initTime->value();
    unsigned int lifeTime = this->ui->lifeTime->value();
    this->clearObjects();
    int rows = this->tableObjectRowCount();
    if (!this->checkObjects(rows) || !this->checkPatterns()) return;
    DistributionType distributionType = this->distributionFromIndex(this->ui->cmbTypeDistribution->currentIndex());
    QAbstractItemModel* model = this->ui->tableObjects->model();
    for (int i = 0; i < rows; ++i)
    {
        QString strType = model->data(model->index(i, 0)).toString();
        ObjectCategory type = TaticalMovingObject::typeFromString(strType.toStdString());
        int total = model->data(model->index(i, 1)).toInt();        
        for (int j = 0; j < total; ++j)
        {
            if (airObjectsType.count(type) > 0)
            {
                this->airManager.add(type, distributionType, initialTime, lifeTime);
            }
            else if (landObjectsType.count(type) > 0)
            {
                this->landManager.add(type, distributionType, initialTime, lifeTime);
            }
            else if (waterObjectsType.count(type) > 0
                     || underwaterObjectsType.count(type) > 0)
            {
                this->waterManager.add(type, distributionType, initialTime, lifeTime);
            }
        }
    }
    QString strType = this->getFirstValidObject();
    this->updateObjectList(strType);
    this->updateAirObjects();
    this->updateLandObjects();
    this->updateWaterObjects();
    ui->toolBox->setCurrentIndex(0);
    this->on_objectsID_activated(0);
    this->airCanvas->clear();
    this->airCanvas->repaint();
    this->repaint();    
}

void MainWindow::on_timeSlider_valueChanged(int value)
{
    this->airCanvas->clear();
    this->repaint();
}

void MainWindow::on_AddPattern_clicked()
{
    QAbstractItemModel* model = this->ui->patternTable->model();
    if (model == nullptr)
    {
        model = new QStandardItemModel(this);
        this->ui->patternTable->setModel(model);
        model->insertColumn(0, QModelIndex());
        model->insertColumn(1, QModelIndex());
        model->setHeaderData(0, Qt::Horizontal, QObject::tr("Pattern"));
        model->setHeaderData(1, Qt::Horizontal, QObject::tr("Time"));
    }
    model->insertRow(0);
    model->setData(model->index(0, 0), this->ui->airMovingPattern->currentText());
    model->setData(model->index(0, 1), this->ui->spinBoxAirPST->value());
    this->ui->patternTable->resizeColumnsToContents();
    this->ui->spinBoxAirPST->setValue(this->ui->spinBoxAirPST->value() + 1.);
}


double MainWindow::getMaxVelocity(const ObjectCategory &type)
{
    if (airObjectsType.count(type) > 0)
    {
        return this->ui->maxVelAir->value();
    }
    else if (landObjectsType.count(type) > 0)
    {
        return this->ui->maxVelCar->value();
    }
    else if (waterObjectsType.count(type) > 0 || underwaterObjectsType.count(type) > 0)
    {
        return this->ui->maxVelShip->value();
    }
    return 0.0;
}

double MainWindow::getMinVelocity(const ObjectCategory &type)
{
    if (airObjectsType.count(type) > 0)
    {
        return this->ui->minVelAir->value();
    }
    else if (landObjectsType.count(type) > 0)
    {
        return this->ui->minVelCar->value();
    }
    else if (waterObjectsType.count(type) > 0 || underwaterObjectsType.count(type) > 0)
    {
        return this->ui->minVelShip->value();
    }
    return 0.0;
}

double MainWindow::getMaxAcceleration(const ObjectCategory &type)
{
    if (airObjectsType.count(type) > 0)
    {
        return this->ui->maxAccelAir->value();
    }
    else if (landObjectsType.count(type) > 0)
    {
        return this->ui->maxAccelCar->value();
    }
    else if (waterObjectsType.count(type) > 0 || underwaterObjectsType.count(type) > 0)
    {
        return this->ui->maxAccelShip->value();
    }
    return 0.0;
}

double MainWindow::getMinAcceleration(const ObjectCategory &type)
{
    if (airObjectsType.count(type) > 0)
    {
        return this->ui->minAccelAir->value();
    }
    else if (landObjectsType.count(type) > 0)
    {
        return this->ui->minAccelCar->value();
    }
    else if (waterObjectsType.count(type) > 0 || underwaterObjectsType.count(type) > 0)
    {
        return this->ui->minAccelShip->value();
    }
    return 0.0;
}

double MainWindow::getMaxAltitude(const ObjectCategory &type)
{
    if (airObjectsType.count(type) > 0)
    {
        return this->ui->maxAltAir->value();
    }
    return 0.0;
}

double MainWindow::getMinAltitude(const ObjectCategory &type)
{
    if (airObjectsType.count(type) > 0)
    {
        return this->ui->minAltAir->value();
    }
    return 0.0;
}

double MainWindow::getMaxDepth(const ObjectCategory &type)
{
    if (underwaterObjectsType.count(type) > 0)
    {
        return this->ui->maxDepthShip->value();
    }
    return 0.0;
}

double MainWindow::getMinDepth(const ObjectCategory &type)
{
    if (underwaterObjectsType.count(type) > 0)
    {
        return this->ui->minDepthShip->value();
    }
    return 0.0;
}

unsigned int MainWindow::getInitialTime()
{
    return this->ui->initTime->value();
}

unsigned int MainWindow::getLifeTime()
{
    return this->ui->lifeTime->value();
}

QString object2name(TaticalMovingObject* object)
{    
    return QString("%1_%2").arg(object->getName()).arg(object->getID());
}

QString object2type(TaticalMovingObject* object)
{
    return QString::fromStdString(TaticalMovingObject::stringFromType(object->getType()));
}

std::pair<double, double> objectAltitude(TaticalMovingObject* object)
{
    if (airObjectsType.count(object->getType()))
    {
        Air *air = ((Air*)object);
        return std::make_pair(air->getMaxAltitude(), air->getMinAltitude());
    }
    return std::make_pair(0.0, 0.0);
}

std::pair<double, double> objectDepth(TaticalMovingObject* object)
{
    if (underwaterObjectsType.count(object->getType()))
    {
        Water *ship = ((Water*)object);
        return std::make_pair(ship->getMaxDepth(), ship->getMinDepth());
    }
    return std::make_pair(0.0, 0.0);
}


void MainWindow::on_pushButton_clicked()
{
    QString output = QFileDialog::getSaveFileName(this, tr("Save positions"),"",
                                                  tr("Positions (*.csv);;All Files (*)"));
    if (!output.isEmpty())
    {
        QFile file(output);
        if (!file.open(QIODevice::WriteOnly)) {
            QMessageBox::information(this, tr("Unable to open file"),
                file.errorString());
            return;
        }
        QTextStream out(&file);
        out << "time,object_name,position_x,position_y,position_z,velocity_x,velocity_y,velocity_z,acceleration_x,acceleration_y,acceleration_z,max_altitude,min_altitude,max_depth,min_depth,initial_time,end_time,pattern_switch_time,moving_pattern\r\n";
        for (auto kv: this->objects)
        {
            auto object = kv.second;
            for (int t = 0; t < this->lifetimeValue; ++t)
            {
                Pair p = this->dataManager.getData(t, object->getID());
                Coordinates position = p.first;
                Vector velocity = p.second;
                QString patternStr = QString::fromStdString(Pattern::stringFromPattern(object->getPattern(t)));
                out << t
                    << ","
                    << object2name(object)
                    << ","
                    << position.getX()
                    << ","
                    << position.getY()
                    << ","
                    << position.getZ()
                    << ","
                    << velocity.getX()
                    << ","
                    << velocity.getY()
                    << ","
                    << velocity.getZ()
                    << ","
                    << object->getAcceleraton().getX()
                    << ","
                    << object->getAcceleraton().getY()
                    << ","
                    << object->getAcceleraton().getZ()
                    << ","
                    << objectAltitude(object).first // max altitude
                    << ","
                    << objectAltitude(object).second // min altitude
                    << ","
                    << objectDepth(object).first // max depth
                    << ","
                    << objectDepth(object).second // min depth
                    << ","
                    << object->getInitialTime()
                    << ","
                    << object->getEndTime()
                    << ","
                    << t
                    << ","
                    << patternStr;
                 out << "\r\n";
            }
        }
        file.close();
        QMessageBox::information(this, tr("Export to CSV"), tr("csv file saved!"));
    }
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    QModelIndexList selectedRows = this->positionTable->selectionModel()->selectedIndexes();
    if (!selectedRows.isEmpty())
    {
        if (event->matches(QKeySequence::Copy))
        {
           QString text;

           for (int i = 0; i < selectedRows.count(); ++i)
           {
               QModelIndex index = selectedRows[i];
               text += index.data().toString();
               if (index.column() < 2) text += '\t';
               else text += '\n';
           }
           QApplication::clipboard()->setText(text);
        }
    }
}

int MainWindow::getObjectCount()
{
    return this->stationObjects.size();
}

void MainWindow::initStationTab()
{
    QVBoxLayout *layout = static_cast<QVBoxLayout *>(ui->tab_3->layout());

    sceneNK = new QGraphicsScene(this);
    graphicsViewNK = new GraphicsView(GraphicsViewType::NK, this);
    graphicsViewNK->setStyleSheet("background-color:rgb(165, 180, 222);");
    graphicsViewNK->setAcceptDrops(true);
    graphicsViewNK->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    graphicsViewNK->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    graphicsViewNK->setScene(sceneNK);
    layout->insertWidget(1, graphicsViewNK);

    sceneSK = new QGraphicsScene(this);
    graphicsViewSK = new GraphicsView(GraphicsViewType::SK, this);
    graphicsViewSK->setStyleSheet("background-color:rgb(165, 180, 222);");
    graphicsViewSK->setAcceptDrops(true);
    graphicsViewSK->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    graphicsViewSK->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    graphicsViewSK->setScene(sceneSK);
    layout->insertWidget(3, graphicsViewSK);

    shipNK = new DragWidget(DragWidgetType::ShipNK);
    shipSK = new DragWidget(DragWidgetType::ShipSK);
    airPortNK = new DragWidget(DragWidgetType::AirportNK);
    airPortSK = new DragWidget(DragWidgetType::AirportSK);
    helicopterCarNK = new DragWidget(DragWidgetType::HelicopterCarNK);
    helicopterCarSK = new DragWidget(DragWidgetType::HelicopterCarSK);

    ui->tbInsertObject->addWidget(shipNK);
    ui->tbInsertObject->addWidget(airPortNK);
    ui->tbInsertObject->addWidget(helicopterCarNK);

    QFrame* line0 = new QFrame();
    line0->setFrameShape(QFrame::VLine);
    line0->setFrameShadow(QFrame::Sunken);
    ui->tbInsertObject->addWidget(line0);

    ui->tbInsertObject->addWidget(shipSK);
    ui->tbInsertObject->addWidget(airPortSK);
    ui->tbInsertObject->addWidget(helicopterCarSK);

    QFrame* line1 = new QFrame();
    line1->setFrameShape(QFrame::VLine);
    line1->setFrameShadow(QFrame::Sunken);
    ui->tbInsertObject->addWidget(line1);

    btnGenerate = new QPushButton("Generate");
    ui->tbInsertObject->addWidget(btnGenerate);
    connect(btnGenerate, SIGNAL(clicked()), this, SLOT(onStationObjectGenerateClicked()));

    btnAnimate = new QPushButton("Animate");
    ui->tbInsertObject->addWidget(btnAnimate);
    btnAnimate->setEnabled(false);
    connect(btnAnimate, SIGNAL(clicked()), this, SLOT(onAnimateClicked()));

    btnExportCSV = new QPushButton("Export CSV file");
    ui->tbInsertObject->addWidget(btnExportCSV);
    connect(btnExportCSV, SIGNAL(clicked()), this, SLOT(onExportCSVClicked()));

    ui->tbInsertObject->addStretch();
}

void MainWindow::onStationObjectGenerateClicked()
{
    StationSetting *dlg = new StationSetting(this);
    dlg->show();
}

void MainWindow::generateStationObjects(int shipNK, int helicopterNK, int carNK, int airplaneNK, int shipSK, int helicopterSK, int carSK, int airplaneSK)
{  
    for (TaticalMovingObject* obj: this->stationObjects)
    {
        if (obj != nullptr) delete obj;
    }
    this->stationObjects.clear();
    unsigned int initialTime = this->ui->initTime->value();
    unsigned int lifeTime = this->ui->lifeTime->value();

    if (ui->patternTable->model()->rowCount() == 0)
    {
        QMessageBox messageBox;
        messageBox.critical(0,"Error","You must adds at least one moving pattern");
        messageBox.setFixedSize(500,200);
        return;
    }

    DistributionType distributionType = DistributionType::Uniform;
    int index = this->ui->cmbTypeDistribution->currentIndex();
    switch(index)
    {
    case 0:
        break;
    case 1:
        distributionType = DistributionType::Poisson;
        break;
    case 2:
        distributionType = DistributionType::LeftSkewed;
        break;
    case 3:
        distributionType = DistributionType::RightSkewed;
        break;
    default:
        QMessageBox messageBox;
        messageBox.critical(0,"Error","Invalid Distribution Type!");
        messageBox.setFixedSize(500,200);
        return;
    }
    QRectF whiteArea;
    GraphicsView::getWhiteArea(World, whiteArea);

    for (int i = 0; i < airplaneNK; ++i)
    {
        ObjectCategory type = ObjectCategory::PassengerAirPlane;
        Air* air = static_cast<Air*>(ObjectGenerator::generateStationObject(GraphicsViewType::NK, World, whiteArea, type, distributionType, getMaxVelocity(type), getMinVelocity(type), getMaxAcceleration(type), getMinAcceleration(type)));
        air->setMaxAltitude(getMaxAltitude(type));
        air->setMinAltitude(getMinAltitude(type));
        this->stationObjects.push_back(air);
    }
    for (int i = 0; i < airplaneSK; ++i)
    {
        ObjectCategory type = ObjectCategory::PassengerAirPlane;
        Air* ap = static_cast<Air *>(ObjectGenerator::generateStationObject(GraphicsViewType::SK, World, whiteArea, type, distributionType, getMaxVelocity(type), getMinVelocity(type), getMaxAcceleration(type), getMinAcceleration(type)));
        ap->setMaxAltitude(getMaxAltitude(type));
        ap->setMinAltitude(getMinAltitude(type));
        this->stationObjects.push_back(ap);
    }

    for (int i = 0; i < helicopterNK; ++i)
    {
        ObjectCategory type = ObjectCategory::Helicopter;
        Helicopter* hc = static_cast<Helicopter *>(ObjectGenerator::generateStationObject(GraphicsViewType::NK, World, whiteArea, type, distributionType, getMaxVelocity(type), getMinVelocity(type), getMaxAcceleration(type), getMinAcceleration(type)));
        hc->setMaxAltitude(getMaxAltitude(type));
        hc->setMinAltitude(getMinAltitude(type));
        hc->setRotationAngle(ui->RA->value());
        hc->setRotationStart(ui->RAST->value());
        this->stationObjects.push_back(hc);
    }
    for (int i = 0; i < helicopterSK; ++i)
    {
        ObjectCategory type = ObjectCategory::Helicopter;
        Helicopter* hc = static_cast<Helicopter *>(ObjectGenerator::generateStationObject(GraphicsViewType::SK, World, whiteArea, type, distributionType, getMaxVelocity(type), getMinVelocity(type), getMaxAcceleration(type), getMinAcceleration(type)));
        hc->setMaxAltitude(getMaxAltitude(type));
        hc->setMinAltitude(getMinAltitude(type));
        hc->setRotationAngle(ui->RA->value());
        hc->setRotationStart(ui->RAST->value());
        this->stationObjects.push_back(hc);
    }

    for (int i = 0; i < carNK; ++i)
    {
        ObjectCategory type = ObjectCategory::Vehicle;
        Land* car = static_cast<Land *>(ObjectGenerator::generateStationObject(GraphicsViewType::NK, World, whiteArea, type, distributionType, getMaxVelocity(type), getMinVelocity(type), getMaxAcceleration(type), getMinAcceleration(type)));
        this->stationObjects.push_back(car);
    }
    for (int i = 0; i < carSK; ++i)
    {
        ObjectCategory type = ObjectCategory::Vehicle;
        Land* car = static_cast<Land *>(ObjectGenerator::generateStationObject(GraphicsViewType::SK, World, whiteArea, type, distributionType, getMaxVelocity(type), getMinVelocity(type), getMaxAcceleration(type), getMinAcceleration(type)));
        this->stationObjects.push_back(car);
    }

    for (int i = 0; i < shipNK; ++i)
    {
        ObjectCategory type = ObjectCategory::NavalShip;
        Water* sp = static_cast<Water *>(ObjectGenerator::generateStationObject(GraphicsViewType::NK, World, whiteArea, type, distributionType, getMaxVelocity(type), getMinVelocity(type), getMaxAcceleration(type), getMinAcceleration(type)));
        sp->setMaxDepth(getMaxDepth(type));
        sp->setMinDepth(getMinDepth(type));
        this->stationObjects.push_back(sp);
    }
    for (int i = 0; i < shipSK; ++i)
    {
        ObjectCategory type = ObjectCategory::NavalShip;
        Water* sp = static_cast<Water *>(ObjectGenerator::generateStationObject(GraphicsViewType::SK, World, whiteArea, type, distributionType, getMaxVelocity(type), getMinVelocity(type), getMaxAcceleration(type), getMinAcceleration(type)));
        sp->setMaxDepth(getMaxDepth(type));
        sp->setMinDepth(getMinDepth(type));
        this->stationObjects.push_back(sp);
    }

    QStandardItemModel* model = (QStandardItemModel*)this->ui->patternTable->model();
    if (model)
    {
        for (auto obj: this->stationObjects)
        {
            MovingPattern pattern = MovingPattern::Const_Velocity;
            unsigned int instant = 0;
            QStandardItemModel* model = (QStandardItemModel*)this->ui->patternTable->model();
            for (int i = 0; i < model->rowCount(); ++i)
            {
                QString patternStr = model->data(model->index(i, 0)).toString();
                QString instantStr = model->data(model->index(i, 1)).toString();
                instant = instantStr.toInt();
                if (patternStr == "Constant Velocity") {
                    pattern = MovingPattern::Const_Velocity;
                } else if (patternStr == "Constant Acceleration") {
                    pattern = MovingPattern::Const_Acceleration;
                } else if (patternStr == "Constant Deceleration") {
                    pattern = MovingPattern::Const_Deceleration;
                } else {
                    QMessageBox messageBox;
                    messageBox.critical(0,"Error","Invalid Distribution Type!");
                    messageBox.setFixedSize(500,200);
                    return;
                }
                obj->addPattern(pattern, instant);
            }
        }
    } else {
        QMessageBox messageBox;
        messageBox.critical(0,"Error","patternTable' model is null");
        messageBox.setFixedSize(500,200);
        return;
    }

    displayStationObjects();
}


void MainWindow::displayStationObjects()
{
    sceneNK->clear();
    sceneSK->clear();
    graphicsViewNK->clear();
    graphicsViewSK->clear();
    graphicsViewNK->update();
    graphicsViewSK->update();

    graphicsViewNK->drawWhiteArea();
    graphicsViewSK->drawWhiteArea();

    double worldWidth = World.width();
    double worldHeight = World.height() / 2.0;

    double widthNK = graphicsViewNK->width();
    double heightNK = graphicsViewNK->height();

    double widthSK = graphicsViewSK->width();
    double heightSK = graphicsViewSK->height();

    for (const auto obj : this->stationObjects) {
        QString fileName;
        ObjectCategory type = obj->getType();
        switch (type) {
        case ObjectCategory::PassengerAirPlane:
            fileName = ":/images/airplane_";
            break;
        case ObjectCategory::Helicopter:
            fileName = ":/images/helicopter_";
            break;
        case ObjectCategory::Vehicle:
            fileName = ":/images/car_";
            break;
        case ObjectCategory::NavalShip:
            fileName = ":/images/ship_";
            break;
        }
        double x = obj->getPosition().getX();
        double y = obj->getPosition().getY();
        if (y > worldHeight) {
            fileName += "nk.png";
            x = x / worldWidth * widthNK;
            y = (2.0 * worldHeight - y) / worldHeight * heightNK;
            QGraphicsPixmapItem *item = new QGraphicsPixmapItem(QPixmap(fileName));
            item->setPos(x, y);
            sceneNK->addItem(item);
        } else {
            fileName += "sk.png";
            x = x / worldWidth * widthSK;
            y = (worldHeight - y) / worldHeight * heightSK;
            QGraphicsPixmapItem *item = new QGraphicsPixmapItem(QPixmap(fileName));
            item->setPos(x, y);
            sceneSK->addItem(item);
        }
    }
}

void MainWindow::onExportCSVClicked()
{
    if (this->stationObjects.size() == 0) {
        QMessageBox messageBox;
        messageBox.critical(0,"Error","Please generate the objects");
        messageBox.setFixedSize(500,200);
        return;
    }

    std::vector<Station> listStationNK, listStationSK;
    graphicsViewNK->getAllStation(listStationNK);
    graphicsViewSK->getAllStation(listStationSK);

    std::vector<Station> listShipNK, listHelicopterCarNK, listAirportNK, listShipSK, listHelicopterCarSK, listAirportSK;
    for (auto s : listStationNK) {
        switch(s.type) {
        case DragWidgetType::AirportNK:
            listAirportNK.emplace_back(s);
            break;
        case DragWidgetType::ShipNK:
            listShipNK.emplace_back(s);
            break;
        case DragWidgetType::HelicopterCarNK:
            listHelicopterCarNK.emplace_back(s);
            break;
        default:
            QMessageBox messageBox;
            messageBox.critical(0,"Error","Unrecognized NK-area station");
            messageBox.setFixedSize(500,200);
            return;
        }
    }
    if (listAirportNK.size() < 2 || listHelicopterCarNK.size() < 2 || listShipNK.size() < 2) {
        QMessageBox messageBox;
        messageBox.critical(0,"Error","You have to inserte NK-area station more than 2");
        messageBox.setFixedSize(500,200);
        return;
    }
    for (auto s : listStationSK) {
        switch(s.type) {
        case DragWidgetType::AirportSK:
            listAirportSK.emplace_back(s);
            break;
        case DragWidgetType::ShipSK:
            listShipSK.emplace_back(s);
            break;
        case DragWidgetType::HelicopterCarSK:
            listHelicopterCarSK.emplace_back(s);
            break;
        default:
            QMessageBox messageBox;
            messageBox.critical(0,"Error","Unrecognized SK-area station");
            messageBox.setFixedSize(500,200);
            return;
        }
    }
    if (listAirportSK.size() < 2 || listHelicopterCarSK.size() < 2 || listShipSK.size() < 2) {
        QMessageBox messageBox;
        messageBox.critical(0,"Error","You have to inserte SK-area station more than 2");
        messageBox.setFixedSize(500,200);
        return;
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> urdHelicopterCarNK(0, listHelicopterCarNK.size() - 1);
    std::uniform_int_distribution<> urdAirportNK(0, listAirportNK.size() - 1);
    std::uniform_int_distribution<> urdShipNK(0, listShipNK.size() - 1);
    std::uniform_int_distribution<> urdHelicopterCarSK(0, listHelicopterCarSK.size() - 1);
    std::uniform_int_distribution<> urdAirportSK(0, listAirportSK.size() - 1);
    std::uniform_int_distribution<> urdShipSK(0, listShipSK.size() - 1);
    QString output = QFileDialog::getSaveFileName(this, tr("Save positions"),"",
                                                  tr("Positions (*.csv);;All Files (*)"));
    if (!output.isEmpty())
    {
        QFile file(output);
        if (!file.open(QIODevice::WriteOnly)) {
            QMessageBox::information(this, tr("Unable to open file"),
                file.errorString());
            return;
        }
        QTextStream out(&file);
        out << "id,name,type,velocity x,velocity y,velocity z,acceleration x,acceleration y,acceleration z,position x,position y,position z,source,destination,Area,status,lifetime,moving pattern,pattern_switch_time,max_altitude,min_altitude,max_depth,min_depth,initial time,end time\r\n";
        for (auto obj: this->stationObjects)
        {
            // id
            out << obj->getID() << ",";
            // name
            out << object2name(obj) << ",";
            // type
            out << object2type(obj) << ",";
            // velocity x, y, z
            out << obj->getVelocity().getX() << ",";
            out << obj->getVelocity().getY() << ",";
            out << obj->getVelocity().getZ() << ",";
            // acceleration x, y, z
            out << obj->getAcceleraton().getX() << ",";
            out << obj->getAcceleraton().getY() << ",";
            out << obj->getAcceleraton().getZ() << ",";
            // position x, y, z
            out << obj->getPosition().getX() << ",";
            out << obj->getPosition().getY() << ",";
            out << obj->getPosition().getZ() << ",";

            GraphicsViewType areaType = obj->getAreaType();
            ObjectCategory objType = obj->getType();
            double lifeTime = 0;
            QString source, destination, area, status;
            int indexSrc, indexDst;
            if (areaType == GraphicsViewType::NK) {
                area = "NK-area";
                switch (objType) {
                case ObjectCategory::Helicopter:
                case ObjectCategory::Vehicle:
                    indexSrc = urdHelicopterCarNK(gen);
                    indexDst = indexSrc;
                    while(indexSrc == indexDst) {
                        indexDst = urdHelicopterCarNK(gen);
                    }
                    source = QString("%0th Helicopter & Car station").arg(indexSrc);
                    destination = QString("%0th Helicopter & Car station").arg(indexDst);
                    lifeTime = calcLifeTime(obj, listHelicopterCarNK[indexSrc], listHelicopterCarNK[indexDst], status);
                    break;
                case ObjectCategory::PassengerAirPlane:
                    indexSrc = urdAirportNK(gen);
                    indexDst = indexSrc;
                    while(indexSrc == indexDst) {
                        indexDst = urdAirportNK(gen);
                    }
                    source = QString("%0th Airport base").arg(indexSrc);
                    destination = QString("%0th Airport base").arg(indexDst);
                    lifeTime = calcLifeTime(obj, listAirportNK[indexSrc], listAirportNK[indexDst], status);
                    break;
                case ObjectCategory::NavalShip:
                    indexSrc = urdShipNK(gen);
                    indexDst = indexSrc;
                    while(indexSrc == indexDst) {
                        indexDst = urdShipNK(gen);
                    }
                    source = QString("%0th Ship base").arg(indexSrc);
                    destination = QString("%0th Ship base").arg(indexDst);
                    lifeTime = calcLifeTime(obj, listShipNK[indexSrc], listShipNK[indexDst], status);
                    break;
                }
            } else {
                area = "SK-area";
                switch (objType) {
                case ObjectCategory::Helicopter:
                case ObjectCategory::Vehicle:
                    indexSrc = urdHelicopterCarSK(gen);
                    indexDst = indexSrc;
                    while(indexSrc == indexDst) {
                        indexDst = urdHelicopterCarSK(gen);
                    }
                    source = QString("%0th Helicopter & Car station").arg(indexSrc);
                    destination = QString("%0th Helicopter & Car station").arg(indexDst);
                    lifeTime = calcLifeTime(obj, listHelicopterCarSK[indexSrc], listHelicopterCarSK[indexDst], status);
                    break;
                case ObjectCategory::PassengerAirPlane:
                    indexSrc = urdAirportSK(gen);
                    indexDst = indexSrc;
                    while(indexSrc == indexDst) {
                        indexDst = urdAirportSK(gen);
                    }
                    source = QString("%0th Airport base").arg(indexSrc);
                    destination = QString("%0th Airport base").arg(indexDst);
                    lifeTime = calcLifeTime(obj, listAirportSK[indexSrc], listAirportSK[indexDst], status);
                    break;
                case ObjectCategory::NavalShip:
                    indexSrc = urdShipSK(gen);
                    indexDst = indexSrc;
                    while(indexSrc == indexDst) {
                        indexDst = urdShipSK(gen);
                    }
                    source = QString("%0th Ship base").arg(indexSrc);
                    destination = QString("%0th Ship base").arg(indexDst);
                    lifeTime = calcLifeTime(obj, listShipSK[indexSrc], listShipSK[indexDst], status);
                    break;
                }
            }
            // source,destination,Area
            out << source << ",";
            out << destination << ",";
            out << area << ",";
            // status,lifetime
            out << status << ",";
            out << lifeTime << ",";
            // moving pattern
            QString patternStr = QString::fromStdString(Pattern::stringFromPattern(obj->getPattern(0)));
            out << patternStr << ",";
            // pattern_switch_time
            out << "0" << ",";
            // max_altitude
            out << objectAltitude(obj).first << ",";
            // min_altitude
            out << objectAltitude(obj).second << ",";
            // max_depth
            out << objectDepth(obj).first << ",";
            // min_depth
            out << objectDepth(obj).second << ",";
            // initial time
            out << "0" << ",";
            // end time
            out << lifeTime;

            out << "\r\n";
        }
        file.close();
        QMessageBox::information(this, tr("Export to CSV"), tr("csv file saved!"));
    }
}

double MainWindow::calcLifeTime(TaticalMovingObject *obj, const Station &src, const Station &dst, QString &status)
{
    double worldWidth = World.width();
    double worldHeight = World.height() / 2.0;

    double widthNK = graphicsViewNK->width();
    double heightNK = graphicsViewNK->height();

    double widthSK = graphicsViewSK->width();
    double heightSK = graphicsViewSK->height();

    double xVelocity = obj->getVelocity().getX();
    double yVelocity = obj->getVelocity().getY();
    double xAccel = obj->getAcceleraton().getX();
    double yAccel = obj->getAcceleraton().getY();
    double xSrc = src.pos.x(), ySrc = src.pos.y(), xDst = dst.pos.x(), yDst = dst.pos.y();
    double yObj = obj->getPosition().getY();
    GraphicsViewType areaType = obj->getAreaType();
    if (areaType == GraphicsViewType::NK) {
        if (yObj < worldHeight || fabs(yObj - worldHeight) < 100.0) {
            status = "warning";
        } else {
            status = "normal";
        }
        xSrc = xSrc / widthNK * worldWidth;
        xDst = xDst / widthNK * worldWidth;

        ySrc = (heightNK - ySrc) / heightNK * worldHeight + worldHeight;
        yDst = (heightNK - yDst) / heightNK * worldHeight + worldHeight;
    } else {
        if (yObj > worldHeight || fabs(yObj - worldHeight) < 100.0) {
            status = "warning";
        } else {
            status = "normal";
        }
        xSrc = xSrc / widthSK * worldWidth;
        xDst = xDst / widthSK * worldWidth;

        ySrc = (heightSK - ySrc) / heightSK * worldHeight;
        yDst = (heightSK - yDst) / heightSK * worldHeight;
    }
    double v = sqrtf(xVelocity * xVelocity + yVelocity * yVelocity);
    double a = sqrtf(xAccel * xAccel + yAccel * yAccel);
    double s = sqrtf((xDst - xSrc) * (xDst - xSrc) + (yDst - ySrc) * (yDst - ySrc));
    if (a < DBL_MIN) {
        if (v < DBL_MIN) {
            return -1.0;
        } else {
            return s / v;
        }
    } else {
        if (v < DBL_MIN) {
            return sqrtf(2.0 * s / a);
        } else {
            return (-v + sqrtf(v * v + 4.0 * a * s)) / (2.0 * a);
        }
    }

    return 0.0;
}

void MainWindow::on_pushButton_3_clicked()
{
    QAbstractItemModel* model = this->ui->tableObjects->model();
    if (model)
    {
        model->insertRow(0);
        QString type = this->ui->airComboBox->currentText();
        model->setData(model->index(0, 0), type);
        model->setData(model->index(0, 1), this->ui->airSB->value());
        model->setData(model->index(0, 2), this->ui->maxVelAir->value());
        model->setData(model->index(0, 3), this->ui->minVelAir->value());
        model->setData(model->index(0, 4), this->ui->maxAccelAir->value());
        model->setData(model->index(0, 5), this->ui->minAccelAir->value());
        model->setData(model->index(0, 6), this->ui->maxAltAir->value());
        model->setData(model->index(0, 7), this->ui->minAltAir->value());
        if (type == "Helicopter")
        {
            model->setData(model->index(0, 8), this->ui->RA->value());
            model->setData(model->index(0, 9), this->ui->RAST->value());
        }
        model->setData(model->index(0, 12), this->ui->airCollectedEach->currentText());
    }
}

void MainWindow::on_pushButton_4_clicked()
{
    QAbstractItemModel* model = this->ui->tableObjects->model();
    if (model)
    {
        model->insertRow(0);
        ObjectCategory category = TaticalMovingObject::typeFromString(this->ui->landSubcategory->currentText().toStdString());
        QString type = QString::fromStdString(TaticalMovingObject::stringFromType(category));
        model->setData(model->index(0, 0), type);
        model->setData(model->index(0, 1), this->ui->landSB->value());
        model->setData(model->index(0, 2), this->ui->maxVelCar->value());
        model->setData(model->index(0, 3), this->ui->minVelCar->value());
        model->setData(model->index(0, 4), this->ui->maxAccelCar->value());
        model->setData(model->index(0, 5), this->ui->minAccelCar->value());
        model->setData(model->index(0, 12), this->ui->landCollectedEach->currentText());
    }
}

void MainWindow::on_pushButton_5_clicked()
{
    QAbstractItemModel* model = this->ui->tableObjects->model();
    if (model)
    {
        model->insertRow(0);
        QString type = this->ui->navalComboBox->currentText();
        model->setData(model->index(0, 0), type);
        model->setData(model->index(0, 1), this->ui->navalSB->value());
        model->setData(model->index(0, 2), this->ui->maxVelShip->value());
        model->setData(model->index(0, 3), this->ui->minVelShip->value());
        model->setData(model->index(0, 4), this->ui->maxAccelShip->value());
        model->setData(model->index(0, 5), this->ui->minAccelShip->value());
        if (type == "Underwater")
        {
            model->setData(model->index(0, 10), this->ui->maxDepthShip->value());
            model->setData(model->index(0, 11), this->ui->minDepthShip->value());
        }
        model->setData(model->index(0, 12), this->ui->waterCollectedEach->currentText());
    }
}

void MainWindow::depthEnable(bool flag)
{
    this->ui->maxDepthShip->setEnabled(flag);
    this->ui->maxDepthLabel->setEnabled(flag);
    this->ui->minDepthShip->setEnabled(flag);
    this->ui->minDepthLabel->setEnabled(flag);
}

void MainWindow::on_pushButton_6_clicked()
{
    QStandardItemModel *model =  (QStandardItemModel*)this->ui->tableObjects->model();
    model->clear();
    this->initObjectsTable();    
}

void MainWindow::on_pushButton_7_clicked()
{
    QStandardItemModel *model =  (QStandardItemModel*)this->ui->patternTable->model();
    model->clear();
    this->initPatternTable();
}

void MainWindow::on_objectsID_activated(const QString &arg1)
{
    if (this->objects.size() == 0) {
        return;
    }
    TaticalMovingObject* object = this->objectMap[arg1];
    if (!object) return;
    QString type = QString::fromStdString(TaticalMovingObject::stringFromType(object->getType()));
    this->ui->objectType->setPlainText(type);
    this->ui->objectST->setPlainText(QString::number(object->getInitialTime()));
    this->ui->objectET->setPlainText(QString::number(object->getEndTime()));
    this->chart->removeAllSeries();
    this->series = new QLineSeries();
    for (int t = 0; t < lifetimeValue; ++t)
    {
        Pair p = this->dataManager.getData(t, object->getID());
        Coordinates pos = p.first;
        Vector vel = p.second;
        this->series->append(t, vel.length());
    }
    this->chart->setTitle(QString("%1 %2 - Velocity x Time").arg(type).arg(object->getID()));
    this->chart->addSeries(this->series);
    this->chart->createDefaultAxes();
    this->chart->legend()->setVisible(false);
    this->chartView->repaint();

    QStandardItemModel* model = (QStandardItemModel*)this->positionTable->model();
    if (model)
    {
        model->removeRows(0, this->lifetimeValue);
        for (int t = 0; t < this->lifetimeValue; ++t)
        {
            Pair p = this->dataManager.getData(t, object->getID());
            Coordinates pos = p.first;
            Vector vel = p.second;
            model->insertRow(t);
            model->setData(model->index(t, 0), t);
            model->setData(model->index(t, 1), pos.getX());
            model->setData(model->index(t, 2), pos.getY());
            model->setData(model->index(t, 3), pos.getZ());
            model->setData(model->index(t, 4), QString::fromStdString(Pattern::stringFromPattern(object->getPattern(t))));
        }

    }
    this->ui->patternTable->resizeColumnsToContents();
}

void MainWindow::on_objectCategories_activated(const QString &arg1)
{
    this->updateObjectList(arg1);
    if (this->ui->objectsID->model()->rowCount() == 0)
    {
        this->chart->removeAllSeries();
        QStandardItemModel *model = (QStandardItemModel*)this->positionTable->model();
        model->clear();
        this->initPositionTable();
        this->ui->objectType->setPlainText("");
        this->ui->objectST->setPlainText("");
        this->ui->objectET->setPlainText("");
    }
    else
    {
        this->on_objectsID_activated(this->objectMap[this->ui->objectsID->currentText()]->getName());
    }
}

void MainWindow::on_tabWidget_currentChanged(int index)
{
    if (index == 1)
    {
        QString typeStr = "Airplane";
        for (int i = 0; i < this->objects.size(); ++i)
        {
            typeStr = QString::fromStdString(TaticalMovingObject::stringFromType(this->objects.begin()->second->getType()));
            break;
        }
        this->on_objectCategories_activated(typeStr);
    }
}

void MainWindow::rotorEnabled(bool flag)
{
    this->ui->RA->setEnabled(flag);
    this->ui->RALabel->setEnabled(flag);
    this->ui->RAST->setEnabled(flag);
    this->ui->RASTLabel->setEnabled(flag);
}

void MainWindow::on_airComboBox_activated(const QString &arg1)
{
    rotorEnabled(false);
    ObjectCategory category = TaticalMovingObject::typeFromString(arg1.toStdString());
    delete this->air;
    this->air = (AirObject*)(ObjectFactory::make(category));
    if (category == ObjectCategory::Helicopter) rotorEnabled(true);
    QVector<QDoubleSpinBox*> maxObj = {
        this->ui->maxVelAir,
        this->ui->maxAccelAir,
        this->ui->maxAltAir
    };
    QVector<double> maxVal = {
        air->maxVelocity,
        air->maxAcceleration,
        air->maxAltitude
    };
    QVector<QDoubleSpinBox*> minObj = {
        this->ui->minVelAir,
        this->ui->minAccelAir,
        this->ui->minAltAir
    };
    QVector<double> minVal = {
        air->minVelocity,
        air->minAcceleration,
        air->minAltitude
    };
    for (int i = 0; i < maxObj.size(); ++i)
    {
        QDoubleSpinBox *obj = maxObj[i];
        if (!obj) continue;
        obj->setMaximum(maxVal[i]);
        obj->setValue(maxVal[i]);
        obj = minObj[i];
        if (!obj) continue;
        obj->setMinimum(minVal[i]);
        obj->setValue(minVal[i]);
    }
}

void MainWindow::on_landSubcategory_activated(const QString &arg1)
{
    ObjectCategory category = TaticalMovingObject::typeFromString(arg1.toStdString());
    delete this->land;
    this->land = (LandObject*)(ObjectFactory::make(category));
    QVector<QDoubleSpinBox*> maxObj = {
        this->ui->maxVelCar,
        this->ui->maxAccelCar
    };
    QVector<double> maxVal = {
        land->maxVelocity,
        land->maxAcceleration
    };
    QVector<QDoubleSpinBox*> minObj = {
        this->ui->minVelCar,
        this->ui->minAccelCar
    };
    QVector<double> minVal = {
        land->minVelocity,
        land->minAcceleration
    };
    for (int i = 0; i < maxObj.size(); ++i)
    {
        QDoubleSpinBox *obj = maxObj[i];
        if (!obj) continue;
        obj->setMaximum(maxVal[i]);
        obj->setValue(maxVal[i]);
        obj = minObj[i];
        if (!obj) continue;
        obj->setMinimum(minVal[i]);
        obj->setValue(minVal[i]);
    }
}

void MainWindow::on_navalComboBox_activated(const QString &arg1)
{
    this->depthEnable(false);
    delete this->water;
    ObjectCategory category = TaticalMovingObject::typeFromString(arg1.toStdString());
    this->water = (WaterObject*)ObjectFactory::make(category);
    QVector<QDoubleSpinBox*> maxObj = {
        this->ui->maxVelShip,
        this->ui->maxAccelShip
    };
    QVector<double> maxVal = {
        this->water->maxVelocity,
        this->water->maxAcceleration
    };
    if (underwaterObjectsType.count(category) > 0)
    {
        UnderwaterObject *under = (UnderwaterObject*)this->water;
        this->depthEnable(true);
        this->ui->maxDepthShip->setMaximum(int(under->maxDepth));
        this->ui->maxDepthShip->setValue(int(under->maxDepth));
        this->ui->minDepthShip->setMaximum(int(under->minDepth));
        this->ui->minDepthShip->setValue(int(under->minDepth));
    }

    QVector<QDoubleSpinBox*> minObj = {
        this->ui->minVelShip,
        this->ui->minAccelShip
    };
    QVector<double> minVal = {
        this->water->minVelocity,
        this->water->minAcceleration,
    };
    for (int i = 0; i < maxObj.size(); ++i)
    {
        QDoubleSpinBox *obj = maxObj[i];
        if (!obj) continue;
        obj->setMaximum(maxVal[i]);
        obj->setValue(maxVal[i]);
        obj = minObj[i];
        if (!obj) continue;
        obj->setMinimum(minVal[i]);
        obj->setValue(minVal[i]);
    }
}


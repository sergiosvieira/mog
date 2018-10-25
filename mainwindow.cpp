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
#include "car.h"
#include "helicopter.h"
#include "ship.h"
#include "objectgenerator.h"

#include <float.h>
#include <memory>
#include "histogram.h"
#include "trajectory3d.h"
#include <QGraphicsPixmapItem>
#include "stationsetting.h"
#include "objecttypefactory.h"

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
    posModel->setHeaderData(0, Qt::Horizontal, QObject::tr("Time"));
    posModel->setHeaderData(1, Qt::Horizontal, QObject::tr("Position X"));
    posModel->setHeaderData(2, Qt::Horizontal, QObject::tr("Position Y"));
    posModel->setHeaderData(3, Qt::Horizontal, QObject::tr("Position Z"));
    this->positionTable->setModel(posModel);

    ui->tab_2->layout()->addWidget(this->positionTable);
    initPatternTable();
    initObjectsTable();
    initStationTab();
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
    posModel->setHeaderData(0, Qt::Horizontal, QObject::tr("Time"));
    posModel->setHeaderData(1, Qt::Horizontal, QObject::tr("Position X"));
    posModel->setHeaderData(2, Qt::Horizontal, QObject::tr("Position Y"));
    posModel->setHeaderData(3, Qt::Horizontal, QObject::tr("Position Z"));
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
        QObject::tr("Min. Depth")
    };
    for (int i = 0; i < titles.size(); ++i)
    {
        model->insertColumn(i, QModelIndex());
        model->setHeaderData(i, Qt::Horizontal, titles[i]);
    }
}

MainWindow::~MainWindow()
{
    for (Object* object: this->objects)
    {
        if (object != nullptr) delete object;
    }
    this->objects.clear();
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
    for (auto object: this->objects)
    {
        Pair p = dataManager.getData(time, object->getID());
        Coordinates position = p.first;
        Coordinates u = transform(position, World, this->airCanvas->rect());
        this->airCanvas->draw(*object, u);
    }
}

void MainWindow::onHistogramClicked()
{
    int count = this->objects.size();
    if (count == 0) {
        return;
    }
    std::vector<std::vector<double>> arrVelocity(this->lifetimeValue);
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
    if (this->objects.size() == 0) {
        return;
    }
    Object *obj = this->objectMap[ui->objectsID->currentText()];
    if (!obj) return;
    ObjectType objType = obj->getType();
    Trajectory3D *trajectoryWindow = new Trajectory3D(this);
    trajectoryWindow->show();
    if (objType == ObjectType::Land || objType == ObjectType::OnWater)
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

int MainWindow::totalObjects()
{
    int result = this->ui->tableObjects->model()->rowCount();
    return result;
}

void MainWindow::updateObjectList(const QString& str)
{
    this->ui->objectsID->clear();
    ObjectType type = Object::typeFromString(str.toStdString());
    bool flag = true;
    int index = 0;
    for (auto object: this->objects)
    {
        if (object->getType() != type) continue;
        if (flag)
        {
            flag = false;
            int index = this->ui->objectCategories->findText(str);
            this->ui->objectCategories->setCurrentIndex(index);
        }
        this->ui->objectsID->addItem(object->getName());
        this->objectMap[object->getName()] = object;
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
}

void MainWindow::on_btn_air_generate_clicked()
{
    this->lifetimeValue = this->ui->lifeTime->value();
    for (Object* object: this->objects)
    {
        if (object != nullptr) delete object;
    }
    this->objects.clear();
    unsigned int initialTime = this->ui->initTime->value();
    unsigned int lifeTime = this->ui->lifeTime->value();
    if (this->totalObjects() == 0)
    {
        QMessageBox messageBox;
        messageBox.critical(0,"Error","You must adds objects first");
        messageBox.setFixedSize(500,200);
        return;
    }
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
        return;
    }
    QAbstractItemModel* model = this->ui->tableObjects->model();
    int n = this->totalObjects();
    QSet<QString> airStr = {
        "Airplane", "Helicopter", "Missile", "Cargo Aircraft", "Boeing Airplane", "Fighter Jet"
    };
    QSet<QString> navalStr = {
        "On water", "Underwater"
    };
    for (int i = 0; i < n; ++i)
    {
        QString strType = model->data(model->index(i, 0)).toString();
        int total = model->data(model->index(i, 1)).toInt();
        ObjectType type = Object::typeFromString(strType.toStdString());
        if (airStr.contains(strType))
        {
            for (int j = 0; j < total; ++j)
            {
                AirPlane* ap = static_cast<AirPlane *>(ObjectGenerator::generate(World,
                                                                                 type,
                                                                                 distributionType,
                                                                                 getMaxVelocity(type),
                                                                                 getMinVelocity(type),
                                                                                 getMaxAcceleration(type),
                                                                                 getMinAcceleration(type),
                                                                                 initialTime,
                                                                                 lifeTime));
                ap->setMaxAltitude(getMaxAltitude(type));
                ap->setMinAltitude(getMinAltitude(type));
                if (strType == "Helicopter")
                {
                    Helicopter* hc = static_cast<Helicopter *>(ObjectGenerator::generate(World,
                                                                                         type,
                                                                                         distributionType,
                                                                                         getMaxVelocity(type),
                                                                                         getMinVelocity(type),
                                                                                         getMaxAcceleration(type),
                                                                                         getMinAcceleration(type),
                                                                                         initialTime,
                                                                                         lifeTime));
                    hc->setRotationAngle(ui->RA->value());
                    hc->setRotationStart(ui->RAST->value());
                }
                this->objects.push_back(ap);
            }
        }
        else if (strType == "Land")
        {
            for (int j = 0; j < total; ++j)
            {
                Car *land = static_cast<Car *>(ObjectGenerator::generate(World, type, distributionType, getMaxVelocity(type), getMinVelocity(type), getMaxAcceleration(type), getMinAcceleration(type), initialTime, lifeTime));
                this->objects.push_back(land);
            }
        }
        else if (navalStr.contains(strType))
        {
            for (int j = 0; j < total; ++j)
            {

                Ship* sp = static_cast<Ship *>(ObjectGenerator::generate(World, type, distributionType, getMaxVelocity(type), getMinVelocity(type), getMaxAcceleration(type), getMinAcceleration(type), initialTime, lifeTime));
                sp->setMaxDepth(getMaxDepth(type));
                sp->setMinDepth(getMinDepth(type));
                this->objects.push_back(sp);
            }
        }
    }
    QString strType = "Airplane";
    if (this->objects.size() > 0)
    {
        auto object =this->objects[0];
        strType = QString::fromStdString(Object::stringFromType(object->getType()));
    }
    this->updateObjectList(strType);    
//    this->ui->objectsID->clear();
//    for (auto object: this->objects)
//    {
//        this->ui->objectsID->addItem(QString("Object %0").arg(object->getID()));
//        MovingPattern pattern = MovingPattern::Const_Velocity;
//        unsigned int instant = 0;
//        QStandardItemModel* model = (QStandardItemModel*)this->ui->patternTable->model();
//        for (int i = 0; i < model->rowCount(); ++i)
//        {
//            QString patternStr = model->data(model->index(i, 0)).toString();
//            QString instantStr = model->data(model->index(i, 1)).toString();
//            instant = instantStr.toInt();
//            if (patternStr == "Constant Velocity") pattern = MovingPattern::Const_Velocity;
//            else if (patternStr == "Constant Acceleration") pattern = MovingPattern::Const_Acceleration;
//            else if (patternStr == "Constant Deceleration") pattern = MovingPattern::Const_Deceleration;
//            object->addPattern(pattern, instant);
//        }
//    }

    for (unsigned int t = 0; t < this->lifetimeValue; ++t)
    {
        for (auto object: this->objects)
        {            
            Coordinates newPosition(0.0, 0.0, 0.0);
            Vector veloc = object->getVelocity();
            if (t == 0)
            {
                newPosition = object->getPosition();
            }
            else
            {
                MovingPattern pattern = object->getPattern(t);
                Vector accel = object->getAcceleraton();
                accel *= 3.6;
                if (pattern == MovingPattern::Const_Velocity)
                {
                    accel = Vector{0., 0., 0.};
                }
                else if (pattern == MovingPattern::Const_Deceleration)
                {
                    accel *= -1.0;
                }
                Pair p = dataManager.getData(t - 1, object->getID());
                newPosition = p.first;
                veloc = p.second;
                veloc += accel;
                newPosition.setX(newPosition.getX() + veloc.getX() * object->getDirection().getX());
                newPosition.setY(newPosition.getY() + veloc.getY() * object->getDirection().getY());
                newPosition.setZ(newPosition.getZ() + veloc.getZ() * object->getDirection().getZ());
            }
            if (t >= object->getInitialTime() && t <= object->getEndTime())
            {
                dataManager.setData(t, object->getID(), std::make_pair(newPosition, veloc));
            }
            else
            {
                Pair p = dataManager.getData(t, object->getID());
                dataManager.setData(t, object->getID(), std::make_pair(p.first, Vector(0.0, 0.0, 0.0)));
            }
        }
    }
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


double MainWindow::getMaxVelocity(const ObjectType &type)
{
    switch (type)
    {
    case ObjectType::AirPlane:
    case ObjectType::Helicopter:
    case ObjectType::Missile:
    case ObjectType::Cargo:
    case ObjectType::Boeing:
    case ObjectType::Fighter:
        return this->ui->maxVelAir->value();
        break;
    case ObjectType::Land:
        return this->ui->maxVelCar->value();
        break;
    case ObjectType::OnWater:
    case ObjectType::Underwater:
        return this->ui->maxVelShip->value();
        break;
    }
    return 0.0;
}

double MainWindow::getMinVelocity(const ObjectType &type)
{
    switch (type)
    {
    case ObjectType::AirPlane:
    case ObjectType::Helicopter:
    case ObjectType::Missile:
    case ObjectType::Cargo:
    case ObjectType::Boeing:
    case ObjectType::Fighter:
        return this->ui->minVelAir->value();
        break;
    case ObjectType::Land:
        return this->ui->minVelCar->value();
        break;
    case ObjectType::OnWater:
    case ObjectType::Underwater:
        return this->ui->minVelShip->value();
        break;
    }
    return 0.0;
}

double MainWindow::getMaxAcceleration(const ObjectType &type)
{
    switch (type)
    {
    case ObjectType::AirPlane:
    case ObjectType::Helicopter:
    case ObjectType::Missile:
    case ObjectType::Cargo:
    case ObjectType::Boeing:
    case ObjectType::Fighter:
        return this->ui->maxAccelAir->value();
        break;
    case ObjectType::Land:
        return this->ui->maxAccelCar->value();
        break;
    case ObjectType::OnWater:
    case ObjectType::Underwater:
        return this->ui->maxAccelShip->value();
        break;
    }
    return 0.0;
}

double MainWindow::getMinAcceleration(const ObjectType &type)
{
    switch (type)
    {
    case ObjectType::AirPlane:
    case ObjectType::Helicopter:
    case ObjectType::Missile:
    case ObjectType::Cargo:
    case ObjectType::Boeing:
    case ObjectType::Fighter:
        return this->ui->minAccelAir->value();
        break;
    case ObjectType::Land:
        return this->ui->minAccelCar->value();
        break;
    case ObjectType::OnWater:
    case ObjectType::Underwater:
        return this->ui->minAccelShip->value();
        break;
    }
    return 0.0;
}

double MainWindow::getMaxAltitude(const ObjectType &type)
{
    switch (type)
    {
    case ObjectType::AirPlane:
    case ObjectType::Helicopter:
    case ObjectType::Missile:
    case ObjectType::Cargo:
    case ObjectType::Boeing:
    case ObjectType::Fighter:
        return this->ui->maxAltAir->value();
        break;
    case ObjectType::Land:
        return 0.0;
        break;
    case ObjectType::OnWater:
    case ObjectType::Underwater:
        return 0.0;
        break;
    }
    return 0.0;
}

double MainWindow::getMinAltitude(const ObjectType &type)
{
    switch (type)
    {
    case ObjectType::AirPlane:
    case ObjectType::Helicopter:
    case ObjectType::Missile:
    case ObjectType::Cargo:
    case ObjectType::Boeing:
    case ObjectType::Fighter:
        return this->ui->minAltAir->value();
        break;
    case ObjectType::Land:
        return 0.0;
        break;
    case ObjectType::OnWater:
    case ObjectType::Underwater:
        return 0.0;
        break;
    }
    return 0.0;
}

double MainWindow::getMaxDepth(const ObjectType &type)
{
    switch (type)
    {
    case ObjectType::AirPlane:
    case ObjectType::Helicopter:
    case ObjectType::Missile:
    case ObjectType::Cargo:
    case ObjectType::Boeing:
    case ObjectType::Fighter:
        return 0.0;
        break;
    case ObjectType::Land:
        return 0.0;
        break;
    case ObjectType::OnWater:
    case ObjectType::Underwater:
        return this->ui->maxDepthShip->value();
        break;
    }
    return 0.0;
}

double MainWindow::getMinDepth(const ObjectType &type)
{
    switch (type)
    {
    case ObjectType::AirPlane:
    case ObjectType::Helicopter:
    case ObjectType::Missile:
    case ObjectType::Cargo:
    case ObjectType::Boeing:
    case ObjectType::Fighter:
        return 0.0;
        break;
    case ObjectType::Land:
        return 0.0;
        break;
    case ObjectType::OnWater:
    case ObjectType::Underwater:
        return this->ui->minDepthShip->value();
        break;
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

QString object2name(Object* object)
{
    switch (object->getType())
    {
        case ObjectType::AirPlane:
            return QString("Airplane_%0").arg(object->getID());
        case ObjectType::Helicopter:
            return QString("Helicopter_%0").arg(object->getID());
        case ObjectType::Missile:
            return QString("Missile_%0").arg(object->getID());
        case ObjectType::Cargo:
            return QString("Cargo_%0").arg(object->getID());
        case ObjectType::Boeing:
            return QString("Boeing_%0").arg(object->getID());
        case ObjectType::Fighter:
            return QString("Fighter_%0").arg(object->getID());
        case ObjectType::Land:
            return QString("Land_%0").arg(object->getID());
        case ObjectType::OnWater:
            return QString("OnWater_%0").arg(object->getID());
        case ObjectType::Underwater:
            return QString("Underwater_%0").arg(object->getID());
    }
    return "";
}

QString object2type(Object* object)
{
    return QString::fromStdString(Object::stringFromType(object->getType()));
}

std::pair<double, double> objectAltitude(Object* object)
{
    switch (object->getType())
    {
        case ObjectType::AirPlane:
        case ObjectType::Helicopter:
        {
            AirObject *air = ((AirObject*)object);
            return std::make_pair(air->getMaxAltitude(), air->getMinAltitude());
        }
    }
    return std::make_pair(0.0, 0.0);
}

std::pair<double, double> objectDepth(Object* object)
{
    switch (object->getType())
    {
        case ObjectType::Underwater:
        {
            Ship *ship = ((Ship*)object);
            return std::make_pair(ship->getMaxDepth(), ship->getMinDepth());
        }
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
        for (auto object: this->objects)
        {
            //std::vector<int> times = object->getAllPatternTime();
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
    for (Object* obj: this->stationObjects)
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
        ObjectType type = ObjectType::AirPlane;
        AirPlane* ap = static_cast<AirPlane *>(ObjectGenerator::generateStationObject(GraphicsViewType::NK, World, whiteArea, type, distributionType, getMaxVelocity(type), getMinVelocity(type), getMaxAcceleration(type), getMinAcceleration(type)));
        ap->setMaxAltitude(getMaxAltitude(type));
        ap->setMinAltitude(getMinAltitude(type));
        this->stationObjects.push_back(ap);
    }
    for (int i = 0; i < airplaneSK; ++i)
    {
        ObjectType type = ObjectType::AirPlane;
        AirPlane* ap = static_cast<AirPlane *>(ObjectGenerator::generateStationObject(GraphicsViewType::SK, World, whiteArea, type, distributionType, getMaxVelocity(type), getMinVelocity(type), getMaxAcceleration(type), getMinAcceleration(type)));
        ap->setMaxAltitude(getMaxAltitude(type));
        ap->setMinAltitude(getMinAltitude(type));
        this->stationObjects.push_back(ap);
    }

    for (int i = 0; i < helicopterNK; ++i)
    {
        ObjectType type = ObjectType::Helicopter;
        Helicopter* hc = static_cast<Helicopter *>(ObjectGenerator::generateStationObject(GraphicsViewType::NK, World, whiteArea, type, distributionType, getMaxVelocity(type), getMinVelocity(type), getMaxAcceleration(type), getMinAcceleration(type)));
        hc->setMaxAltitude(getMaxAltitude(type));
        hc->setMinAltitude(getMinAltitude(type));
        hc->setRotationAngle(ui->RA->value());
        hc->setRotationStart(ui->RAST->value());
        this->stationObjects.push_back(hc);
    }
    for (int i = 0; i < helicopterSK; ++i)
    {
        ObjectType type = ObjectType::Helicopter;
        Helicopter* hc = static_cast<Helicopter *>(ObjectGenerator::generateStationObject(GraphicsViewType::SK, World, whiteArea, type, distributionType, getMaxVelocity(type), getMinVelocity(type), getMaxAcceleration(type), getMinAcceleration(type)));
        hc->setMaxAltitude(getMaxAltitude(type));
        hc->setMinAltitude(getMinAltitude(type));
        hc->setRotationAngle(ui->RA->value());
        hc->setRotationStart(ui->RAST->value());
        this->stationObjects.push_back(hc);
    }

    for (int i = 0; i < carNK; ++i)
    {
        ObjectType type = ObjectType::Land;
        Car* car = static_cast<Car *>(ObjectGenerator::generateStationObject(GraphicsViewType::NK, World, whiteArea, type, distributionType, getMaxVelocity(type), getMinVelocity(type), getMaxAcceleration(type), getMinAcceleration(type)));
        this->stationObjects.push_back(car);
    }
    for (int i = 0; i < carSK; ++i)
    {
        ObjectType type = ObjectType::Land;
        Car* car = static_cast<Car *>(ObjectGenerator::generateStationObject(GraphicsViewType::SK, World, whiteArea, type, distributionType, getMaxVelocity(type), getMinVelocity(type), getMaxAcceleration(type), getMinAcceleration(type)));
        this->stationObjects.push_back(car);
    }

    for (int i = 0; i < shipNK; ++i)
    {
        ObjectType type = ObjectType::OnWater;
        Ship* sp = static_cast<Ship *>(ObjectGenerator::generateStationObject(GraphicsViewType::NK, World, whiteArea, type, distributionType, getMaxVelocity(type), getMinVelocity(type), getMaxAcceleration(type), getMinAcceleration(type)));
        sp->setMaxDepth(getMaxDepth(type));
        sp->setMinDepth(getMinDepth(type));
        this->stationObjects.push_back(sp);
    }
    for (int i = 0; i < shipSK; ++i)
    {
        ObjectType type = ObjectType::OnWater;
        Ship* sp = static_cast<Ship *>(ObjectGenerator::generateStationObject(GraphicsViewType::SK, World, whiteArea, type, distributionType, getMaxVelocity(type), getMinVelocity(type), getMaxAcceleration(type), getMinAcceleration(type)));
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
        ObjectType type = obj->getType();
        switch (type) {
        case ObjectType::AirPlane:
            fileName = ":/images/airplane_";
            break;
        case ObjectType::Helicopter:
            fileName = ":/images/helicopter_";
            break;
        case ObjectType::Land:
            fileName = ":/images/car_";
            break;
        case ObjectType::OnWater:
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
            ObjectType objType = obj->getType();
            double lifeTime = 0;
            QString source, destination, area, status;
            int indexSrc, indexDst;
            if (areaType == GraphicsViewType::NK) {
                area = "NK-area";
                switch (objType) {
                case ObjectType::Helicopter:
                case ObjectType::Land:
                    indexSrc = urdHelicopterCarNK(gen);
                    indexDst = indexSrc;
                    while(indexSrc == indexDst) {
                        indexDst = urdHelicopterCarNK(gen);
                    }
                    source = QString("%0th Helicopter & Car station").arg(indexSrc);
                    destination = QString("%0th Helicopter & Car station").arg(indexDst);
                    lifeTime = calcLifeTime(obj, listHelicopterCarNK[indexSrc], listHelicopterCarNK[indexDst], status);
                    break;
                case ObjectType::AirPlane:
                    indexSrc = urdAirportNK(gen);
                    indexDst = indexSrc;
                    while(indexSrc == indexDst) {
                        indexDst = urdAirportNK(gen);
                    }
                    source = QString("%0th Airport base").arg(indexSrc);
                    destination = QString("%0th Airport base").arg(indexDst);
                    lifeTime = calcLifeTime(obj, listAirportNK[indexSrc], listAirportNK[indexDst], status);
                    break;
                case ObjectType::OnWater:
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
                case ObjectType::Helicopter:
                case ObjectType::Land:
                    indexSrc = urdHelicopterCarSK(gen);
                    indexDst = indexSrc;
                    while(indexSrc == indexDst) {
                        indexDst = urdHelicopterCarSK(gen);
                    }
                    source = QString("%0th Helicopter & Car station").arg(indexSrc);
                    destination = QString("%0th Helicopter & Car station").arg(indexDst);
                    lifeTime = calcLifeTime(obj, listHelicopterCarSK[indexSrc], listHelicopterCarSK[indexDst], status);
                    break;
                case ObjectType::AirPlane:
                    indexSrc = urdAirportSK(gen);
                    indexDst = indexSrc;
                    while(indexSrc == indexDst) {
                        indexDst = urdAirportSK(gen);
                    }
                    source = QString("%0th Airport base").arg(indexSrc);
                    destination = QString("%0th Airport base").arg(indexDst);
                    lifeTime = calcLifeTime(obj, listAirportSK[indexSrc], listAirportSK[indexDst], status);
                    break;
                case ObjectType::OnWater:
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

double MainWindow::calcLifeTime(Object *obj, const Station &src, const Station &dst, QString &status)
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

void MainWindow::on_airplanesSB_valueChanged(int arg1)
{
    double value = this->ui->airSB->value();
    double remain = 100. - value;
    if (remain > 0.)
    {
        double div = remain / 2.0;
        this->ui->landSB->setValue(div);
        this->ui->navalSB->setValue(div);
    }
}

void MainWindow::on_landSB_valueChanged(int arg1)
{
    double value = this->ui->landSB->value();
    double remain = 100. - value;
    if (remain > 0.)
    {
        double div = remain / 2.0;
        this->ui->airSB->setValue(div);
        this->ui->navalSB->setValue(div);
    }

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
    }
}

void MainWindow::on_airComboBox_currentIndexChanged(const QString &arg1)
{
    QString type = "Air";
    this->ui->RA->setEnabled(false);
    this->ui->RALabel->setEnabled(false);
    this->ui->RAST->setEnabled(false);
    this->ui->RASTLabel->setEnabled(false);
    delete this->air;
    if (arg1 == "Airplane")
    {
        this->air = ObjectInfoFactory::makeAirplane();
    }
    else if (arg1 == "Helicopter")
    {
        this->air = ObjectInfoFactory::makeHelicopter();
        this->ui->RA->setEnabled(true);
        this->ui->RALabel->setEnabled(true);
        this->ui->RAST->setEnabled(true);
        this->ui->RASTLabel->setEnabled(true);
    }
    else if (arg1 == "Missile")
    {
        this->air = ObjectInfoFactory::makeMissile();
    }
    else if (arg1 == "Cargo Aircraft")
    {
        this->air = ObjectInfoFactory::makeCargo();
    }
    else if (arg1 == "Fighter Jet")
    {
        this->air = ObjectInfoFactory::makeFighter();
    }
    else if (arg1 == "Boeing Airplane")
    {
        this->air = ObjectInfoFactory::makeBoeing();
    }
    if (type == "Air")
    {
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
    qDebug() << arg1;
}

void MainWindow::on_pushButton_4_clicked()
{
    QAbstractItemModel* model = this->ui->tableObjects->model();
    if (model)
    {
        model->insertRow(0);
        QString type = "Land";
        model->setData(model->index(0, 0), type);
        model->setData(model->index(0, 1), this->ui->landSB->value());
        model->setData(model->index(0, 2), this->ui->maxVelCar->value());
        model->setData(model->index(0, 3), this->ui->minVelCar->value());
        model->setData(model->index(0, 4), this->ui->maxAccelCar->value());
        model->setData(model->index(0, 5), this->ui->minAccelCar->value());
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
    }
}

void MainWindow::on_navalComboBox_currentIndexChanged(const QString &arg1)
{
    this->ui->maxDepthShip->setEnabled(false);
    this->ui->maxDepthLabel->setEnabled(false);
    this->ui->minDepthShip->setEnabled(false);
    this->ui->minDepthLabel->setEnabled(false);
    delete this->naval;
    if (arg1 == "On water")
    {
        this->naval = ObjectInfoFactory::makeOnWater();
    }
    else if (arg1 == "Underwater")
    {
        this->naval = ObjectInfoFactory::makeUnderwater();
        this->ui->maxDepthShip->setEnabled(true);
        this->ui->maxDepthLabel->setEnabled(true);
        this->ui->minDepthShip->setEnabled(true);
        this->ui->minDepthLabel->setEnabled(true);
    }
    QVector<QDoubleSpinBox*> maxObj = {
        this->ui->maxVelShip,
        this->ui->maxAccelShip
    };
    QVector<double> maxVal = {
        naval->maxVelocity,
        naval->maxAcceleration
    };
    if (arg1 == "Underwater")
    {
        UnderwaterInfo *under = (UnderwaterInfo*)(this->naval);
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
        naval->minVelocity,
        naval->minAcceleration,
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
    qDebug() << arg1;

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
    Object* object = this->objectMap[arg1];
    if (!object) return;
    QString type = QString::fromStdString(Object::stringFromType(object->getType()));
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

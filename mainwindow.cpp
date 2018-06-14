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
#include <random>
#include "canvas.h"
#include "airplane.h"
#include "car.h"
#include "helicopter.h"
#include "ship.h"
#include "objectgenerator.h"


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
        dstRect.topLeft().y() + dstRect.height() * ry
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
    posModel->setHeaderData(0, Qt::Horizontal, QObject::tr("Time"));
    posModel->setHeaderData(1, Qt::Horizontal, QObject::tr("Position X"));
    posModel->setHeaderData(2, Qt::Horizontal, QObject::tr("Position Y"));
    this->positionTable->setModel(posModel);
    ui->tab_2->layout()->addWidget(this->positionTable);
    connect(this->positionTable, SIGNAL(keyPressEvent()), SLOT(keyPressEvent()));

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

}

MainWindow::~MainWindow()
{
    for (Object* object: this->objects)
    {
        if (object != nullptr) delete object;
    }
    this->objects.clear();
    delete ui;
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

void MainWindow::on_btn_air_generate_clicked()
{
    for (Object* object: this->objects)
    {
        if (object != nullptr) delete object;
    }
    this->objects.clear();
    unsigned int initialTime = this->ui->initTime->value();
    unsigned int lifeTime = this->ui->lifeTime->value();

    unsigned int total = this->ui->number->value();
    unsigned int airplanes = this->ui->airplanesSB->value();
    unsigned int helicopters= this->ui->helicoptersSB->value();
    unsigned int cars = this->ui->carsSB->value();
    unsigned int ships = this->ui->shipsSB->value();
    if (airplanes + helicopters + cars + ships != total)
    {
        QMessageBox messageBox;
        messageBox.critical(0,"Error","Invalid number of elements!");
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
    for (int i = 0; i < this->ui->airplanesSB->value(); ++i)
    {
        ObjectType type = ObjectType::AirPlane;
        Object* object = ObjectGenerator::generate(World,
                                                   type,
                                                   getMaxVelocity(type),
                                                   getMinVelocity(type),
                                                   getMaxAcceleration(type),
                                                   getMinAcceleration(type),
                                                   initialTime, lifeTime);
        ((AirPlane*)object)->setMaxAltitude(getMaxAltitude(type));
        ((AirPlane*)object)->setMinAltitude(getMinAltitude(type));
        this->objects.push_back(object);
    }
    for (int i = 0; i < this->ui->helicoptersSB->value(); ++i)
    {
        ObjectType type = ObjectType::Helicopter;
        Object* object = ObjectGenerator::generate(World,
                                                   type,
                                                   getMaxVelocity(type),
                                                   getMinVelocity(type),
                                                   getMaxAcceleration(type),
                                                   getMinAcceleration(type),
                                                   initialTime, lifeTime);

        ((Helicopter*)object)->setMaxAltitude(getMaxAltitude(type));
        ((Helicopter*)object)->setMinAltitude(getMinAltitude(type));
        ((Helicopter*)object)->setRotationAngle(ui->RA->value());
        ((Helicopter*)object)->setRotationStart(ui->RAST->value());
        this->objects.push_back(object);
    }
    for (int i = 0; i < this->ui->carsSB->value(); ++i)
    {
        ObjectType type = ObjectType::Car;
        Object* object = ObjectGenerator::generate(World,
                                                   type,
                                                   getMaxVelocity(type),
                                                   getMinVelocity(type),
                                                   getMaxAcceleration(type),
                                                   getMinAcceleration(type),
                                                   initialTime, lifeTime);

        this->objects.push_back(object);
    }
    for (int i = 0; i < this->ui->shipsSB->value(); ++i)
    {
        ObjectType type = ObjectType::Ship;
        Object* object = ObjectGenerator::generate(World,
                                                   type,
                                                   getMaxVelocity(type),
                                                   getMinVelocity(type),
                                                   getMaxAcceleration(type),
                                                   getMinAcceleration(type),
                                                   initialTime, lifeTime);
        ((Ship*)object)->setMaxDepth(getMaxDepth(type));
        ((Ship*)object)->setMinDepth(getMinDepth(type));
        this->objects.push_back(object);
    }
    QStandardItemModel* model = (QStandardItemModel*)this->ui->patternTable->model();
    if (model)
    {
        this->ui->objectsID->clear();
        for (auto object: this->objects)
        {
            this->ui->objectsID->addItem(QString("Object %0").arg(object->getID()));
            MovingPattern pattern = MovingPattern::Const_Velocity;
            unsigned int instant = 0;
            QStandardItemModel* model = (QStandardItemModel*)this->ui->patternTable->model();
            for (int i = 0; i < model->rowCount(); ++i)
            {
                QString patternStr = model->data(model->index(i, 0)).toString();
                QString instantStr = model->data(model->index(i, 1)).toString();
                instant = instantStr.toInt();
                if (patternStr == "Constant Velocity") pattern = MovingPattern::Const_Velocity;
                else if (patternStr == "Constant Acceleration") pattern = MovingPattern::Const_Acceleration;
                else if (patternStr == "Constant Deceleration") pattern = MovingPattern::Const_Deceleration;
                object->addPattern(pattern, instant);
            }
        }
    }
    for (unsigned int t = 0; t < 24; ++t)
    {
        for (auto object: this->objects)
        {            
            Coordinates newPosition;
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
                    accel *= 0.0;
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
            }
            if (t >= object->getInitialTime()
                    && t <= object->getEndTime())
            {
                dataManager.setData(t, object->getID(), std::make_pair(newPosition, veloc));
            }
            else
            {
                Pair p = dataManager.getData(t, object->getID());
                dataManager.setData(t, object->getID(), std::make_pair(p.first, Vector(0.0, 0.0)));
            }
        }
    }
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

void MainWindow::on_objectsID_activated(const QString &arg1)
{

}

void MainWindow::on_objectsID_activated(int index)
{
    Object* object = this->objects[index];
    if (object)
    {
        QString type = "Airplane";
        switch (object->getType()) {
        case ObjectType::Car:
            type = "Car";
            break;
        case ObjectType::Helicopter:
            type = "Helicopter";
            break;
        case ObjectType::Ship:
            type = "Ship";
            break;
        }
        this->ui->objectType->setPlainText(type);
        this->ui->objectST->setPlainText(QString::number(object->getInitialTime()));
        this->ui->objectET->setPlainText(QString::number(object->getEndTime()));
        this->chart->removeAllSeries();
        this->series = new QLineSeries();
        for (int t = 0; t < 24; ++t)
        {
            Pair p = this->dataManager.getData(t, object->getID());
            Coordinates pos = p.first;
            Vector vel = p.second;
            this->series->append(t, vel.length());
        }
        this->chart->setTitle(QString("Object %1 - Velocity x Time").arg(object->getID()));
        this->chart->addSeries(this->series);
        this->chart->createDefaultAxes();;
        this->chart->legend()->setVisible(false);
        this->chartView->repaint();

        QStandardItemModel* model = (QStandardItemModel*)this->positionTable->model();
        if (model)
        {
            model->removeRows(0, 23);
            for (int t = 0; t < 24; ++t)
            {
                Pair p = this->dataManager.getData(t, object->getID());
                Coordinates pos = p.first;
                Vector vel = p.second;
                model->insertRow(t);
                model->setData(model->index(t, 0), t);
                model->setData(model->index(t, 1), pos.getX());
                model->setData(model->index(t, 2), pos.getY());
            }

        }
        this->ui->patternTable->resizeColumnsToContents();


    }
}

double MainWindow::getMaxVelocity(const ObjectType &type)
{
    switch (type)
    {
    case ObjectType::AirPlane:
        return this->ui->maxVelAir->value();
        break;
    case ObjectType::Helicopter:
        return this->ui->maxVelHeli->value();
        break;
    case ObjectType::Car:
        return this->ui->maxVelHeli->value();
        break;
    case ObjectType::Ship:
        return this->ui->maxVelHeli->value();
        break;
    }
    return 0.0;
}

double MainWindow::getMinVelocity(const ObjectType &type)
{
    switch (type)
    {
    case ObjectType::AirPlane:
        return this->ui->minVelAir->value();
        break;
    case ObjectType::Helicopter:
        return this->ui->minVelHeli->value();
        break;
    case ObjectType::Car:
        return this->ui->minVelCar->value();
        break;
    case ObjectType::Ship:
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
        return this->ui->maxAccelAir->value();
        break;
    case ObjectType::Helicopter:
        return this->ui->maxAccelHeli->value();
        break;
    case ObjectType::Car:
        return this->ui->maxAccelCar->value();
        break;
    case ObjectType::Ship:
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
        return this->ui->minAccelAir->value();
        break;
    case ObjectType::Helicopter:
        return this->ui->minAccelHeli->value();
        break;
    case ObjectType::Car:
        return this->ui->minAccelCar->value();
        break;
    case ObjectType::Ship:
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
        return this->ui->maxAltAir->value();
        break;
    case ObjectType::Helicopter:
        return this->ui->maxAltHeli->value();
        break;
    case ObjectType::Car:
        return 0.0;
        break;
    case ObjectType::Ship:
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
        return this->ui->minAltAir->value();
        break;
    case ObjectType::Helicopter:
        return this->ui->minAltHeli->value();
        break;
    case ObjectType::Car:
        return 0.0;
        break;
    case ObjectType::Ship:
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
        return 0.0;
        break;
    case ObjectType::Helicopter:
        return 0.0;
        break;
    case ObjectType::Car:
        return 0.0;
        break;
    case ObjectType::Ship:
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
        return 0.0;
        break;
    case ObjectType::Helicopter:
        return 0.0;
        break;
    case ObjectType::Car:
        return 0.0;
        break;
    case ObjectType::Ship:
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

QString object2str(Object* object)
{
    switch (object->getType())
    {
        case ObjectType::AirPlane:
            return QString("Airplane_%0").arg(object->getID());
        case ObjectType::Helicopter:
            return QString("Helicopter_%0").arg(object->getID());
        case ObjectType::Car:
            return QString("Car_%0").arg(object->getID());
        case ObjectType::Ship:
            return QString("Ship_%0").arg(object->getID());
    }
    return "";
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
        case ObjectType::Ship:
        {
            Ship *ship = ((Ship*)object);
            return std::make_pair(ship->getMaxDepth(), ship->getMinDepth());
        }
    }
    return std::make_pair(0.0, 0.0);
}

QString pattern2Str(MovingPattern pattern)
{
    switch (pattern)
    {
        case MovingPattern::Const_Velocity:
            return "Constant_Velocity";
        case MovingPattern::Const_Acceleration:
            return "Constant_Acceleration";
        case MovingPattern::Const_Deceleration:
            return "Constant_Deceleration";
    }
    return "";
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
        out << "time,object_name,position_x,position_y,velocity_x,velocity_y,acceleration_x,acceleration_y,max_altitude,min_altitude,max_depth,min_depth,initial_time,end_time,pattern_switch_time,moving_pattern\r\n";
        for (auto object: this->objects)
        {
            //std::vector<int> times = object->getAllPatternTime();
            for (int t = 0; t < 24; ++t)
            {
                Pair p = this->dataManager.getData(t, object->getID());
                Coordinates position = p.first;
                Vector velocity = p.second;
                out << t
                    << ","
                    << object2str(object)
                    << ","
                    << position.getX()
                    << ","
                    << position.getY()
                    << ","
                    << velocity.getX()
                    << ","
                    << velocity.getY()
                    << ","
                    << object->getAcceleraton().getX()
                    << ","
                    << object->getAcceleraton().getY()
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
                    << pattern2Str(object->getPattern(t));
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
               QStandardItemModel* model = (QStandardItemModel*)this->positionTable->model();
               QStringList rowContents;
               for (int j = 0; j < 3; ++j)
               {
                   rowContents << model->index(i,j).data().toString();
               }
               text += rowContents.join("\t");
               text += "\n";
           }
           QApplication::clipboard()->setText(text);
        }
    }
}

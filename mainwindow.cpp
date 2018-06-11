#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <random>
#include "canvas.h"
#include "airplane.h"
#include "car.h"
#include "helicopter.h"
#include "ship.h"

static std::random_device rdev{};
static std::mt19937 eng{rdev()};

static const QRect World{0, 0, 19200000, 1920000};

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


double rndMinMax(uint16_t min, uint16_t max)
{
    std::uniform_real_distribution<double> urd(min, max);
    return urd(eng);
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
    airCanvas = new Canvas();
    ui->air_hlayout->addWidget(airCanvas);
    landCanvas = new Canvas();
    ui->land_hlayout->addWidget(landCanvas);
    waterCanvas = new Canvas();
    ui->water_hlayout->addWidget(waterCanvas);    
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    int time = this->ui->timeSlider->value();
    for (auto object: this->airplanes)
    {
        Coordinates position = airManager.getData(time, object.getID());
        Coordinates u = transform(position, World, this->airCanvas->rect());
        this->airCanvas->draw(object, u);
    }
}

void MainWindow::on_btn_air_generate_clicked()
{
    this->airplanes.clear();
    for (int i = 0; i < this->ui->spinBoxAirNumber->value(); ++i)
    {
        double minVelX = rndMinMax(this->ui->spinBoxAirMinVel->value(), this->ui->spinBoxAirMaxVel->value());
        double maxVelX = rndMinMax(minVelX, this->ui->spinBoxAirMaxVel->value());
        double minVelY = rndMinMax(this->ui->spinBoxAirMinVel->value(), this->ui->spinBoxAirMaxVel->value());
        double maxVelY = rndMinMax(minVelX, this->ui->spinBoxAirMaxVel->value());
        if (maxVelX > this->ui->spinBoxAirMaxVel->maximum()) maxVelX = this->ui->spinBoxAirMaxVel->value();
        if (maxVelY > this->ui->spinBoxAirMaxVel->maximum()) maxVelY = this->ui->spinBoxAirMaxVel->value();
        double minAccelX = rndMinMax(this->ui->spinBoxAirMinAccel->value(), this->ui->spinBoxAirMaxAccel->value());
        double maxAccelX = rndMinMax(minAccelX, this->ui->spinBoxAirMaxAccel->value());
        double minAccelY = rndMinMax(this->ui->spinBoxAirMinAccel->value(), this->ui->spinBoxAirMaxAccel->value());
        double maxAccelY = rndMinMax(minAccelX, this->ui->spinBoxAirMaxAccel->value());
        if (maxAccelX > this->ui->spinBoxAirMaxAccel->maximum()) maxAccelX = this->ui->spinBoxAirMaxAccel->value();
        if (maxAccelY > this->ui->spinBoxAirMaxAccel->maximum()) maxAccelY = this->ui->spinBoxAirMaxAccel->value();
        AirPlane airplane
        (
            Coordinates(rndMinMax(0, World.width()), rndMinMax(0, World.height())),
            Vector(rndMinMax(minVelX, maxVelX), rndMinMax(minVelY, maxVelY)),
            0,
            Vector(rndMinMax(minAccelX, maxAccelX), rndMinMax(minAccelY, maxAccelY))
        );
        airplane.setID(i + 1);
        this->airplanes.push_back(airplane);
    }
    for (unsigned int t = 0; t < 24; ++t)
    {
        for (auto object: this->airplanes)
        {
            Coordinates newPosition;
            if (t == 0)
            {
                newPosition = object.getPosition();
            }
            else
            {

                double px_ = airManager.getData(t - 1, object.getID()).getX();
                double py_ = airManager.getData(t - 1, object.getID()).getY();
                double px = px_ + object.getVelocity().getX() + object.getAcceleraton().getX() / 2.0;
                double py = py_ + object.getVelocity().getY() + object.getAcceleraton().getY() / 2.0;
                newPosition = Coordinates(px, py);
            }
            airManager.setData(t, object.getID(), newPosition);
        }
        int k = t;
    }
    this->repaint();
}

void MainWindow::on_timeSlider_valueChanged(int value)
{
    this->airCanvas->clear();
    this->repaint();
}

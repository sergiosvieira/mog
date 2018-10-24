#include "stationsetting.h"
#include "ui_stationsetting.h"
#include "mainwindow.h"

StationSetting::StationSetting(MainWindow *parent) :
    QDialog(parent),
    ui(new Ui::StationSetting)
{
    ui->setupUi(this);

    mainWindow = parent;
    shipNK = helicopterNK = carNK = airplaneNK = 0;
    shipSK = helicopterSK = carSK = airplaneSK = 0;

    validator = new QIntValidator(0, 10000, this);

    ui->edtAirplaneNK->setValidator(validator);
    ui->edtCarNK->setValidator(validator);
    ui->edtHelicopterNK->setValidator(validator);
    ui->edtShipNK->setValidator(validator);
    ui->edtTotalNK->setValidator(validator);

    ui->edtAirplaneSK->setValidator(validator);
    ui->edtCarSK->setValidator(validator);
    ui->edtHelicopterSK->setValidator(validator);
    ui->edtShipSK->setValidator(validator);
    ui->edtTotalSK->setValidator(validator);

    connect(ui->edtAirplaneNK, SIGNAL(editingFinished()), this, SLOT(updateValues()));
    connect(ui->edtCarNK, SIGNAL(editingFinished()), this, SLOT(updateValues()));
    connect(ui->edtHelicopterNK, SIGNAL(editingFinished()), this, SLOT(updateValues()));
    connect(ui->edtShipNK, SIGNAL(editingFinished()), this, SLOT(updateValues()));

    connect(ui->edtAirplaneSK, SIGNAL(editingFinished()), this, SLOT(updateValues()));
    connect(ui->edtCarSK, SIGNAL(editingFinished()), this, SLOT(updateValues()));
    connect(ui->edtHelicopterSK, SIGNAL(editingFinished()), this, SLOT(updateValues()));
    connect(ui->edtShipSK, SIGNAL(editingFinished()), this, SLOT(updateValues()));

    connect(ui->btnOK, SIGNAL(clicked()), this, SLOT(onOKClicked()));
    connect(ui->btnCancel, SIGNAL(clicked()), this, SLOT(onCancelClicked()));

    updateValues();
}

StationSetting::~StationSetting()
{
    delete validator;
    delete ui;
}

void StationSetting::updateValues()
{
    shipNK = ui->edtShipNK->text().toInt();
    helicopterNK = ui->edtHelicopterNK->text().toInt();
    carNK = ui->edtCarNK->text().toInt();
    airplaneNK = ui->edtAirplaneNK->text().toInt();

    shipSK = ui->edtShipSK->text().toInt();
    helicopterSK = ui->edtHelicopterSK->text().toInt();
    carSK = ui->edtCarSK->text().toInt();
    airplaneSK = ui->edtAirplaneSK->text().toInt();

    int totalNK = shipNK + helicopterNK + carNK + airplaneNK;
    int totalSK = shipSK + helicopterSK + carSK + airplaneSK;

    ui->edtTotalNK->setText(QString::number(totalNK));
    ui->edtTotalSK->setText(QString::number(totalSK));
}

void StationSetting::onOKClicked()
{
    updateValues();
    mainWindow->generateStationObjects(shipNK, helicopterNK, carNK, airplaneNK, shipSK, helicopterSK, carSK, airplaneSK);
    this->close();
}

void StationSetting::onCancelClicked()
{
    this->close();
}

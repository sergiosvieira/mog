#ifndef STATIONSETTING_H
#define STATIONSETTING_H

#include <QDialog>
#include <QIntValidator>
#include <QIntValidator>

namespace Ui {
class StationSetting;
}

class MainWindow;
class StationSetting : public QDialog
{
    Q_OBJECT

public:
    explicit StationSetting(MainWindow *parent);
    ~StationSetting();
private slots:
    void updateValues();
    void onOKClicked();
    void onCancelClicked();
private:
    Ui::StationSetting *ui;

    MainWindow *mainWindow;
    QValidator *validator;

    int shipNK, helicopterNK, carNK, airplaneNK;
    int shipSK, helicopterSK, carSK, airplaneSK;
};

#endif // STATIONSETTING_H

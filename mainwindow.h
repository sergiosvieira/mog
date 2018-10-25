#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <QMainWindow>
#include <QTimer>
#include <vector>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include "object.h"

static const QRect World{0, 0, 192000, 192000};


#include "datamanager.h"
#include <QGraphicsScene>
#include "dragwidget.h"
#include <QPushButton>
#include "graphicsview.h"
#include "objecttype.h"


class Canvas;
class QTableView;

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
    VectorOfObject stationObjects;
    QChart *chart;
    QLineSeries *series;
    QChartView *chartView;
    QTableView *positionTable = nullptr;
    AirInfo *air = new AirplaneInfo();
    LandInfo *land = new LandInfo();
    NavalInfo *naval = new NavalInfo();
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void paintEvent(QPaintEvent* event);
    double getMaxVelocity(const ObjectType &type);
    double getMinVelocity(const ObjectType &type);
    double getMaxAcceleration(const ObjectType &type);
    double getMinAcceleration(const ObjectType &type);
    double getMaxAltitude(const ObjectType &type);
    double getMinAltitude(const ObjectType &type);
    double getMaxDepth(const ObjectType &type);
    double getMinDepth(const ObjectType &type);
    unsigned int getInitialTime();
    unsigned int getLifeTime();
    int getObjectCount();
    void generateStationObjects(int shipNK, int helicopterNK, int carNK, int airplaneNK, int shipSK, int helicopterSK, int carSK, int airplaneSK);
private slots:
    void on_btn_air_generate_clicked();
    void on_timeSlider_valueChanged(int value);
    void on_AddPattern_clicked();    
    void on_pushButton_clicked();
    void keyPressEvent(QKeyEvent *event);
    void onHistogramClicked();
    void onTrajectoryClicked();
    void onExportCSVClicked();
    void onStationObjectGenerateClicked();
    void on_airplanesSB_valueChanged(int arg1);
    void on_landSB_valueChanged(int arg1);    
    void on_pushButton_3_clicked();
    void on_airComboBox_currentIndexChanged(const QString &arg1);
    void on_pushButton_4_clicked();
    void on_pushButton_5_clicked();
    void on_navalComboBox_currentIndexChanged(const QString &arg1);
    void on_pushButton_6_clicked();
    void on_pushButton_7_clicked();
    void on_objectsID_activated(const QString &arg1);
    void on_objectCategories_activated(const QString &arg1);

private:
    std::map<QString, Object*> objectMap;
    Ui::MainWindow *ui;

    QGraphicsScene *sceneNK, *sceneSK;
    GraphicsView *graphicsViewNK, *graphicsViewSK;
    DragWidget *shipNK, *shipSK, *airPortNK, *airPortSK, *helicopterCarNK, *helicopterCarSK;
    QPushButton *btnExportCSV, *btnGenerate, *btnAnimate;
    int lifetimeValue = 0;
private:
    void initStationTab();
    double calcLifeTime(Object *obj, const Station &src, const Station &dst, QString &status);
    void displayStationObjects();
    void initObjectsTable();
    int totalObjects();
    void initPatternTable();
    void updateObjectList(const QString &str);
    void initPositionTable();
};

#endif // MAINWINDOW_H

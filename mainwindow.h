#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <QMainWindow>
#include <QTimer>
#include <vector>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>

static const QRect World{0, 0, 192000, 192000};


#include "datamanager.h"
#include <QGraphicsScene>
#include "dragwidget.h"
#include <QPushButton>
#include "graphicsview.h"
#include "objectcategory.h"
#include "distributiontype.h"
#include "kalman.h"
#include "airobjectsmanager.h"
#include "landobjectsmanager.h"
#include "waterobjectsmanager.h"

class TaticalMovingObject;
class Canvas;
class QTableView;

QT_CHARTS_USE_NAMESPACE

namespace Ui {
class MainWindow;
}


using VectorOfObject = std::vector<TaticalMovingObject *>;

/*!
 * \brief The MainWindow class
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT
protected:
    /*!
     * \brief kalman filter
     */
    KalmanFilter k;
    /*!
     * \brief timer
     */
    QTimer* timer = nullptr;
    /*!
     * \brief airCanvas
     */
    Canvas* airCanvas = nullptr;
    /*!
     * \brief dataManager
     */
    DataManager dataManager;
    /*!
     * \brief objects
     */
//    std::map<unsigned int, TaticalMovingObject*> objects;
    /*!
     * \brief stationObjects
     */
    VectorOfObject stationObjects;
    /*!
     * \brief chart
     */
    QChart *chart;
    /*!
     * \brief series
     */
    QLineSeries *series;
    /*!
     * \brief chartView
     */
    QChartView *chartView;
    /*!
     * \brief positionTable
     */
    QTableView *positionTable = nullptr;
    /*!
     * \brief air
     */
    AirObject *air = new AirObject();
    /*!
     * \brief land
     */
    LandObject *land = new LandObject();
    /*!
     * \brief water
     */
    WaterObject *water = new WaterObject();
public:
    /*!
     * \brief MainWindow
     * \param parent
     */
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    /*!
     * \brief paintEvent
     * \param event
     */
    void paintEvent(QPaintEvent* event);
    /*!
     * \brief getMaxVelocity
     * \param type
     * \return
     */
    double getMaxVelocity(const ObjectCategory &type);
    /*!
     * \brief getMinVelocity
     * \param type
     * \return
     */
    double getMinVelocity(const ObjectCategory &type);
    /*!
     * \brief getMaxAcceleration
     * \param type
     * \return
     */
    double getMaxAcceleration(const ObjectCategory &type);
    /*!
     * \brief getMinAcceleration
     * \param type
     * \return
     */
    double getMinAcceleration(const ObjectCategory &type);
    /*!
     * \brief getMaxAltitude
     * \param type
     * \return
     */
    double getMaxAltitude(const ObjectCategory &type);
    /*!
     * \brief getMinAltitude
     * \param type
     * \return
     */
    double getMinAltitude(const ObjectCategory &type);
    /*!
     * \brief getMaxDepth
     * \param type
     * \return
     */
    double getMaxDepth(const ObjectCategory &type);
    /*!
     * \brief getMinDepth
     * \param type
     * \return
     */
    double getMinDepth(const ObjectCategory &type);
    /*!
     * \brief getInitialTime
     * \return
     */
    unsigned int getInitialTime();
    /*!
     * \brief getLifeTime
     * \return
     */
    unsigned int getLifeTime();
    /*!
     * \brief getObjectCount
     * \return
     */
    int getObjectCount();
    /*!
     * \brief generateStationObjects
     * \param shipNK
     * \param helicopterNK
     * \param carNK
     * \param airplaneNK
     * \param shipSK
     * \param helicopterSK
     * \param carSK
     * \param airplaneSK
     */
    void generateStationObjects(int shipNK, int helicopterNK, int carNK, int airplaneNK, int shipSK, int helicopterSK, int carSK, int airplaneSK);
private slots:
    /*!
     * \brief on_btn_air_generate_clicked
     */
    void on_btn_air_generate_clicked();
    /*!
     * \brief on_timeSlider_valueChanged
     * \param value
     */
    void on_timeSlider_valueChanged(int value);
    /*!
     * \brief on_AddPattern_clicked
     */
    void on_AddPattern_clicked();    
    /*!
     * \brief on_pushButton_clicked
     */
    void on_pushButton_clicked();
    /*!
     * \brief keyPressEvent
     * \param event
     */
    void keyPressEvent(QKeyEvent *event);
    /*!
     * \brief onHistogramClicked
     */
    void onHistogramClicked();
    /*!
     * \brief onTrajectoryClicked
     */
    void onTrajectoryClicked();
    /*!
     * \brief onExportCSVClicked
     */
    void onExportCSVClicked();
    /*!
     * \brief onStationObjectGenerateClicked
     */
    void onStationObjectGenerateClicked();  
    /*!
     * \brief on_pushButton_3_clicked
     */
    void on_pushButton_3_clicked();
    /*!
     * \brief on_pushButton_4_clicked
     */
    void on_pushButton_4_clicked();
    /*!
     * \brief on_pushButton_5_clicked
     */
    void on_pushButton_5_clicked();
    /*!
     * \brief on_pushButton_6_clicked
     */
    void on_pushButton_6_clicked();
    /*!
     * \brief on_pushButton_7_clicked
     */
    void on_pushButton_7_clicked();
    /*!
     * \brief on_objectsID_activated
     * \param arg1
     */
    void on_objectsID_activated(const QString &arg1);
    /*!
     * \brief on_objectCategories_activated
     * \param arg1
     */
    void on_objectCategories_activated(const QString &arg1);
    /*!
     * \brief on_tabWidget_currentChanged
     * \param index
     */
    void on_tabWidget_currentChanged(int index);
    /*!
     * \brief on_airComboBox_activated
     * \param arg1
     */
    void on_airComboBox_activated(const QString &arg1);
    /*!
     * \brief on_landSubcategory_activated
     * \param arg1
     */
    void on_landSubcategory_activated(const QString &arg1);
    /*!
     * \brief on_navalComboBox_activated
     * \param arg1
     */
    void on_navalComboBox_activated(const QString &arg1);

private:
    /*!
     * \brief objectMap
     */
    std::map<QString, TaticalMovingObject*> objectMap;
    /*!
     * \brief ui
     */
    Ui::MainWindow *ui;
    /*!
     * \brief sceneNK
     */
    QGraphicsScene *sceneNK, *sceneSK;
    /*!
     * \brief graphicsViewNK
     */
    GraphicsView *graphicsViewNK, *graphicsViewSK;
    /*!
     * \brief shipNK
     */
    DragWidget *shipNK, *shipSK, *airPortNK, *airPortSK, *helicopterCarNK, *helicopterCarSK;
    /*!
     * \brief btnExportCSV
     */
    QPushButton *btnExportCSV, *btnGenerate, *btnAnimate;
    /*!
     * \brief lifetimeValue
     */
    int lifetimeValue = 0;
    AirObjectsManager airManager;
    LandObjectsManager landManager;
    WaterObjectsManager waterManager;
private:
    /*!
     * \brief initStationTab
     */
    void initStationTab();
    /*!
     * \brief calcLifeTime
     * \param obj
     * \param src
     * \param dst
     * \param status
     * \return
     */
    double calcLifeTime(TaticalMovingObject *obj, const Station &src, const Station &dst, QString &status);
    /*!
     * \brief displayStationObjects
     */
    void displayStationObjects();
    /*!
     * \brief initObjectsTable
     */
    void initObjectsTable();
    /*!
     * \brief tableObjectRowCount
     * \return
     */
    int tableObjectRowCount();
    /*!
     * \brief initPatternTable
     */
    void initPatternTable();
    /*!
     * \brief updateObjectList
     * \param str
     */
    void updateObjectList(const QString &str);
    /*!
     * \brief initPositionTable
     */
    void initPositionTable();
    /*!
     * \brief configurePattern
     * \param object
     */
    void configurePattern(TaticalMovingObject *object);
    /*!
     * \brief distributionFromIndex
     * \param index
     * \return
     */
    DistributionType distributionFromIndex(int index);
    /*!
     * \brief clearObjects
     */
    void clearObjects();
    /*!
     * \brief checkObjects
     * \param n
     * \return
     */
    bool checkObjects(int n);
    /*!
     * \brief checkPatterns
     * \return
     */
    bool checkPatterns();
    /*!
     * \brief depthEnable
     * \param flag
     */
    void depthEnable(bool flag);
    /*!
     * \brief rotorEnabled
     * \param flag
     */
    void rotorEnabled(bool flag);
    /*!
     * \brief initKalmanFilter
     */
    void initKalmanFilter();
    /*!
     * \brief getFirstValidObject
     * \return
     */
    QString getFirstValidObject();
    /*!
     * \brief updateAirObjects
     */
    void updateAirObjects();
    /*!
     * \brief updateLandObjects
     */
    void updateLandObjects();
    /*!
     * \brief updateWaterObjects
     */
    void updateWaterObjects();
};

#endif // MAINWINDOW_H

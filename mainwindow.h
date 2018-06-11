#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <vector>

#include "datamanager.h"

class Canvas;


namespace Ui {
class MainWindow;
}

using VectorOfObject = std::vector<Object>;

class MainWindow : public QMainWindow
{
    Q_OBJECT
protected:
    QTimer* timer = nullptr;
    Canvas* airCanvas = nullptr;
    DataManager airManager;
    Canvas* landCanvas = nullptr;
    DataManager landManager;
    Canvas* waterCanvas = nullptr;
    DataManager waterManager;
    VectorOfObject airplanes, helicopters, cars, ships;
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void paintEvent(QPaintEvent* event);
private slots:
    void on_btn_air_generate_clicked();

    void on_timeSlider_valueChanged(int value);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H

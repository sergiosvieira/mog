#-------------------------------------------------
#
# Project created by QtCreator 2018-06-06T08:31:20
#
#-------------------------------------------------

QT       += core gui charts datavisualization

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = mog
TEMPLATE = app

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    object.cpp \
    vector.cpp \
    circularlist.cpp \
    node.cpp \
    movingpattern.cpp \
    datamanager.cpp \
    airplane.cpp \
    helicopter.cpp \
    car.cpp \
    ship.cpp \
    canvas.cpp \
    coordinates.cpp \
    objectgenerator.cpp \
    airobject.cpp \
    histogram.cpp \
    trajectory3d.cpp \
    dragwidget.cpp \
    graphicsview.cpp \
    stationsetting.cpp

HEADERS += \
        mainwindow.h \
    object.h \
    vector.h \
    circularlist.h \
    node.h \
    movingpattern.h \
    datamanager.h \
    airplane.h \
    helicopter.h \
    car.h \
    ship.h \
    canvas.h \
    coordinates.h \
    objectgenerator.h \
    airobject.h \
    histogram.h \
    trajectory3d.h \
    dragwidget.h \
    graphicsview.h \
    stationsetting.h \
    header.h \
    enum.h \
    objecttype.h \
    objecttypefactory.h

FORMS += \
        mainwindow.ui \
    histogram.ui \
    trajectory3d.ui \
    stationsetting.ui

RESOURCES += \
    mog.qrc

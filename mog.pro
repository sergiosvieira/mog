#-------------------------------------------------
#
# Project created by QtCreator 2018-06-06T08:31:20
#
#-------------------------------------------------

QT       += core gui charts datavisualization

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = mog
TEMPLATE = app

INCLUDEPATH += . ./lib/eigenlib

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
    vector.cpp \
    movingpattern.cpp \
    datamanager.cpp \
    airplane.cpp \
    helicopter.cpp \
    canvas.cpp \
    coordinates.cpp \
    objectgenerator.cpp \
    histogram.cpp \
    trajectory3d.cpp \
    dragwidget.cpp \
    graphicsview.cpp \
    stationsetting.cpp \
    taticalmovingobject.cpp \
    air.cpp \
    land.cpp \
    water.cpp \
    taticalmovingobjectsmanager.cpp \
    airobjectsmanager.cpp \
    objectfactory.cpp \
    landobjectsmanager.cpp \
    waterobjectsmanager.cpp \
    kalman.cpp

HEADERS += \
        mainwindow.h \
    vector.h \
    movingpattern.h \
    datamanager.h \
    airplane.h \
    helicopter.h \
    canvas.h \
    coordinates.h \
    objectgenerator.h \
    histogram.h \
    trajectory3d.h \
    dragwidget.h \
    graphicsview.h \
    stationsetting.h \
    enum.h \
    taticalmovingobject.h \
    air.h \
    objectcategory.h \
    land.h \
    objectfactory.h \
    water.h \
    taticalmovingobjectsmanager.h \
    airobjectsmanager.h \
    distributiontype.h \
    dragwidgettype.h \
    graphicsviewtype.h \
    mapcolor.h \
    landobjectsmanager.h \
    waterobjectsmanager.h \
    kalman.h \
    collectedeach.h \
    cg.h

FORMS += \
        mainwindow.ui \
    histogram.ui \
    trajectory3d.ui \
    stationsetting.ui

RESOURCES += \
    mog.qrc

#-------------------------------------------------
#
# Project created by QtCreator 2013-10-14T22:48:52
#
#-------------------------------------------------

QT       += core gui serialport printsupport webkit
CONFIG += qwt

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets webkitwidgets

TARGET = FlexiPlot
TEMPLATE = app

RC_FILE = flexiplot.rc


SOURCES += main.cpp\
        mainwindow.cpp \
    qcustomplot.cpp \
    plotter.cpp \
    plotitem.cpp \
    settings.cpp \
    config.cpp \
    gmapwidget.cpp \
    mapper.cpp \
    abstractwidget.cpp

HEADERS  += mainwindow.h \
    qcustomplot.h \
    plotter.h \
    plotitem.h \
    settings.h \
    config.h \
    gmapwidget.h \
    mapper.h \
    abstractwidget.h

FORMS    += mainwindow.ui \
    plotter.ui \
    settings.ui \
    mapper.ui

RESOURCES += \
    icons.qrc \
    data.qrc

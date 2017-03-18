#-------------------------------------------------
#
# Project created by QtCreator 2013-10-14T22:48:52
#
#-------------------------------------------------

QT       += core gui serialport printsupport charts
CONFIG += qwt

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = FlexiPlot
TEMPLATE = app

RC_FILE = flexiplot.rc


SOURCES += main.cpp\
        mainwindow.cpp \
    widgets/plotitem.cpp \
    settings.cpp \
    config.cpp \
    #gmapwidget.cpp \
    #mapper.cpp \
    widgets/abstractwidget.cpp \
    terminal.cpp \
    widgets/bargraph.cpp \
    widgets/linegraph.cpp \
    debugtools.cpp \
    widgets/helper.cpp

HEADERS  += mainwindow.h \
    widgets/plotitem.h \
    settings.h \
    config.h \
    #gmapwidget.h \
    #mapper.h \
    widgets/abstractwidget.h \
    terminal.h \
    widgets/bargraph.h \
    widgets/linegraph.h \
    debugtools.h \
    widgets/helper.h

FORMS    += mainwindow.ui \
    settings.ui \
    widgets/mapper.ui \
    terminal.ui \
    widgets/bargraph.ui \
    widgets/linegraph.ui \
    debugtools.ui

RESOURCES += \
    icons.qrc \
    data.qrc

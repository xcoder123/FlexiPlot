#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include <QMessageBox>
#include <QTimer>

#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <qwt_legend.h>

#include <QDebug>

#include "plotter.h"
#include "settings.h"
#include "config.h"
#include "abstractwidget.h"
#include "terminal.h"
//#include "mapper.h"

namespace Ui {
class MainWindow;
}




class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void openDash(QString fileName);

private slots:
    void readData();
    void sendData(QByteArray data);
    void openSerialPort();
    void closeSerialPort();
    void addGraph();
    void addMap();
    void deleteGraph();
    void openSettings();
    void showAbout();
    void newDash();
    void normalSave();
    void saveAs();
    void saveDash(QString fileName);
    void openDashDialog();
    void unsavedChanges(bool bb);
    void settingsChanged();
    void openTerminal();


private:
    Ui::MainWindow *ui;
    QSerialPort *serial;
    Settings * settings;

    QByteArray allData;

    QList<AbstractWidget*> plotters;

    int tick;
    int packetsDropped;

    Terminal * terminal;
};

#endif // MAINWINDOW_H

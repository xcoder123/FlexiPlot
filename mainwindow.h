#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#define MAX_RECENT_OPEN_MENU_ENTRIES        10

#include <QMainWindow>
#include <QSerialPort>
#include <QMessageBox>
#include <QTimer>


#include <QDebug>

#include "settings.h"
#include "config.h"
#include "terminal.h"
#include "widgets/helper.h"
#include "debugtools.h"
#include "aboutdialog.h"

#include "widgets/abstractwidget.h"
//#include "widgets/linegraph.h"
#include "widgets/bargraph.h"
#include "widgets/piechart.h"
#include "widgets/linechart.h"
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
    void readSettings();

public slots:
    void openDash(QString fileName);

private slots:
    void readData();
    void sendData(QByteArray data);

    void parseData(QByteArray data);

    void openSerialPort();
    void closeSerialPort();

    void addLineGraph();
    void addLineChart();
    void addBarGraph();
    void addPieChart();
    void addMap();
    void deleteWidget();

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

    void saveSerialSettings();

    void lockStateChanged(bool checked);

    void addToRecentFiles(QString fileName);
    void recentFileMenuItemClicked();
    void clearRecentFilesMenu();

private:
    void constructRecentFilesMenu();


protected:
    void closeEvent(QCloseEvent * e);

private:
    Ui::MainWindow *ui;
    QSerialPort *serial;
    Settings * settings;

    QByteArray allData;

    QList<AbstractWidget*> widgets;

    int tick;
    int packetsDropped;

    Terminal * terminal;
    DebugTools * debugToolsDialog;

    QComboBox * baudrateComboBox;
    QComboBox * serialPortComboBox;

    AboutDialog about;

    QStringList recentFiles;
};

#endif // MAINWINDOW_H

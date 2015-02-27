/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.3.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMdiArea>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionExit;
    QAction *actionConnect;
    QAction *actionDisconnect;
    QAction *actionSettings;
    QAction *actionAdd_Graph;
    QAction *actionAbout;
    QAction *actionNew;
    QAction *actionOpen;
    QAction *actionSave;
    QAction *actionSave_AS;
    QAction *actionCascade;
    QAction *actionTile;
    QAction *actionAdd_Map;
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout_2;
    QMdiArea *mdiArea;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuSerial;
    QMenu *menuGraph;
    QMenu *menuHelp;
    QMenu *menuWindows;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(488, 440);
        QIcon icon;
        icon.addFile(QStringLiteral(":/images/icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        MainWindow->setWindowIcon(icon);
        actionExit = new QAction(MainWindow);
        actionExit->setObjectName(QStringLiteral("actionExit"));
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/images/application-exit.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionExit->setIcon(icon1);
        actionConnect = new QAction(MainWindow);
        actionConnect->setObjectName(QStringLiteral("actionConnect"));
        QIcon icon2;
        icon2.addFile(QStringLiteral(":/images/Connect.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionConnect->setIcon(icon2);
        actionDisconnect = new QAction(MainWindow);
        actionDisconnect->setObjectName(QStringLiteral("actionDisconnect"));
        QIcon icon3;
        icon3.addFile(QStringLiteral(":/images/Disconnect.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionDisconnect->setIcon(icon3);
        actionSettings = new QAction(MainWindow);
        actionSettings->setObjectName(QStringLiteral("actionSettings"));
        QIcon icon4;
        icon4.addFile(QStringLiteral(":/images/settings.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionSettings->setIcon(icon4);
        actionAdd_Graph = new QAction(MainWindow);
        actionAdd_Graph->setObjectName(QStringLiteral("actionAdd_Graph"));
        QIcon icon5;
        icon5.addFile(QStringLiteral(":/images/graph.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionAdd_Graph->setIcon(icon5);
        actionAbout = new QAction(MainWindow);
        actionAbout->setObjectName(QStringLiteral("actionAbout"));
        QIcon icon6;
        icon6.addFile(QStringLiteral(":/images/help-about.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionAbout->setIcon(icon6);
        actionNew = new QAction(MainWindow);
        actionNew->setObjectName(QStringLiteral("actionNew"));
        QIcon icon7;
        icon7.addFile(QStringLiteral(":/images/document-new.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionNew->setIcon(icon7);
        actionOpen = new QAction(MainWindow);
        actionOpen->setObjectName(QStringLiteral("actionOpen"));
        QIcon icon8;
        icon8.addFile(QStringLiteral(":/images/document-open-folder.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionOpen->setIcon(icon8);
        actionSave = new QAction(MainWindow);
        actionSave->setObjectName(QStringLiteral("actionSave"));
        QIcon icon9;
        icon9.addFile(QStringLiteral(":/images/document-save.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionSave->setIcon(icon9);
        actionSave_AS = new QAction(MainWindow);
        actionSave_AS->setObjectName(QStringLiteral("actionSave_AS"));
        QIcon icon10;
        icon10.addFile(QStringLiteral(":/images/document-save-as.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionSave_AS->setIcon(icon10);
        actionCascade = new QAction(MainWindow);
        actionCascade->setObjectName(QStringLiteral("actionCascade"));
        QIcon icon11;
        icon11.addFile(QStringLiteral(":/images/window-duplicate.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionCascade->setIcon(icon11);
        actionTile = new QAction(MainWindow);
        actionTile->setObjectName(QStringLiteral("actionTile"));
        QIcon icon12;
        icon12.addFile(QStringLiteral(":/images/view-split-left-right.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionTile->setIcon(icon12);
        actionAdd_Map = new QAction(MainWindow);
        actionAdd_Map->setObjectName(QStringLiteral("actionAdd_Map"));
        QIcon icon13;
        icon13.addFile(QStringLiteral(":/images/04_maps.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionAdd_Map->setIcon(icon13);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        verticalLayout_2 = new QVBoxLayout(centralWidget);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        mdiArea = new QMdiArea(centralWidget);
        mdiArea->setObjectName(QStringLiteral("mdiArea"));
        mdiArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        mdiArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        mdiArea->setViewMode(QMdiArea::SubWindowView);

        verticalLayout_2->addWidget(mdiArea);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 488, 21));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QStringLiteral("menuFile"));
        menuSerial = new QMenu(menuBar);
        menuSerial->setObjectName(QStringLiteral("menuSerial"));
        menuGraph = new QMenu(menuBar);
        menuGraph->setObjectName(QStringLiteral("menuGraph"));
        menuHelp = new QMenu(menuBar);
        menuHelp->setObjectName(QStringLiteral("menuHelp"));
        menuWindows = new QMenu(menuBar);
        menuWindows->setObjectName(QStringLiteral("menuWindows"));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuSerial->menuAction());
        menuBar->addAction(menuGraph->menuAction());
        menuBar->addAction(menuWindows->menuAction());
        menuBar->addAction(menuHelp->menuAction());
        menuFile->addAction(actionNew);
        menuFile->addAction(actionOpen);
        menuFile->addAction(actionSave);
        menuFile->addAction(actionSave_AS);
        menuFile->addSeparator();
        menuFile->addAction(actionExit);
        menuSerial->addAction(actionConnect);
        menuSerial->addAction(actionDisconnect);
        menuSerial->addAction(actionSettings);
        menuGraph->addAction(actionAdd_Graph);
        menuGraph->addAction(actionAdd_Map);
        menuHelp->addAction(actionAbout);
        menuWindows->addAction(actionCascade);
        menuWindows->addAction(actionTile);
        mainToolBar->addAction(actionNew);
        mainToolBar->addAction(actionOpen);
        mainToolBar->addAction(actionSave);
        mainToolBar->addSeparator();
        mainToolBar->addAction(actionConnect);
        mainToolBar->addAction(actionDisconnect);
        mainToolBar->addAction(actionSettings);
        mainToolBar->addSeparator();
        mainToolBar->addAction(actionAdd_Graph);
        mainToolBar->addAction(actionAdd_Map);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "FlexiPlot[*]", 0));
        actionExit->setText(QApplication::translate("MainWindow", "Exit", 0));
        actionConnect->setText(QApplication::translate("MainWindow", "Connect", 0));
        actionConnect->setShortcut(QApplication::translate("MainWindow", "Ctrl+F", 0));
        actionDisconnect->setText(QApplication::translate("MainWindow", "Disconnect", 0));
        actionDisconnect->setShortcut(QApplication::translate("MainWindow", "Ctrl+D", 0));
        actionSettings->setText(QApplication::translate("MainWindow", "Settings", 0));
        actionAdd_Graph->setText(QApplication::translate("MainWindow", "Add Plot", 0));
        actionAdd_Graph->setShortcut(QApplication::translate("MainWindow", "Ctrl++, Ctrl+=", 0));
        actionAbout->setText(QApplication::translate("MainWindow", "About", 0));
        actionNew->setText(QApplication::translate("MainWindow", "New", 0));
        actionNew->setShortcut(QApplication::translate("MainWindow", "Ctrl+N", 0));
        actionOpen->setText(QApplication::translate("MainWindow", "Open", 0));
        actionOpen->setShortcut(QApplication::translate("MainWindow", "Ctrl+O", 0));
        actionSave->setText(QApplication::translate("MainWindow", "Save", 0));
        actionSave->setShortcut(QApplication::translate("MainWindow", "Ctrl+S", 0));
        actionSave_AS->setText(QApplication::translate("MainWindow", "Save As", 0));
        actionCascade->setText(QApplication::translate("MainWindow", "Cascade", 0));
        actionTile->setText(QApplication::translate("MainWindow", "Tile", 0));
        actionAdd_Map->setText(QApplication::translate("MainWindow", "Add Map", 0));
        menuFile->setTitle(QApplication::translate("MainWindow", "File", 0));
        menuSerial->setTitle(QApplication::translate("MainWindow", "Serial", 0));
        menuGraph->setTitle(QApplication::translate("MainWindow", "Graph", 0));
        menuHelp->setTitle(QApplication::translate("MainWindow", "Help", 0));
        menuWindows->setTitle(QApplication::translate("MainWindow", "Windows", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H

/********************************************************************************
** Form generated from reading UI file 'plotter.ui'
**
** Created by: Qt User Interface Compiler version 5.3.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PLOTTER_H
#define UI_PLOTTER_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include <qwt_plot.h>

QT_BEGIN_NAMESPACE

class Ui_Plotter
{
public:
    QVBoxLayout *verticalLayout;
    QTabWidget *tabWidget;
    QWidget *tab;
    QVBoxLayout *verticalLayout_7;
    QwtPlot *plot;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer;
    QPushButton *startStopPlotBtn;
    QWidget *tab_2;
    QVBoxLayout *verticalLayout_3;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;
    QVBoxLayout *verticalLayout_6;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout_2;
    QFormLayout *formLayout;
    QLabel *label;
    QLineEdit *idEdit;
    QLabel *label_2;
    QComboBox *typeComboBox;
    QLabel *label_5;
    QLineEdit *titleEdit;
    QLabel *label_6;
    QSpinBox *bufferSpin;
    QLabel *label_7;
    QSpinBox *refreshRateSpin;
    QLabel *label_9;
    QLineEdit *xaxisEdit;
    QLabel *label_10;
    QLineEdit *yaxisEdit;
    QGroupBox *groupBox_2;
    QVBoxLayout *verticalLayout_4;
    QFormLayout *formLayout_2;
    QCheckBox *autoScaleYCheckBox;
    QLabel *label_3;
    QDoubleSpinBox *minYSpin;
    QLabel *label_4;
    QDoubleSpinBox *maxYSpin;
    QCheckBox *autoScaleXCheckBox;
    QLabel *label_11;
    QDoubleSpinBox *minXSpin;
    QDoubleSpinBox *maxXSpin;
    QLabel *label_12;
    QGroupBox *groupBox_3;
    QVBoxLayout *verticalLayout_5;
    QFormLayout *formLayout_3;
    QLabel *label_8;
    QLineEdit *dateFormat;
    QCheckBox *fillCheckBox;
    QLabel *label_13;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer_2;
    QLabel *colorLbl;
    QPushButton *colorBtn;
    QLabel *label_14;
    QSpinBox *transparentSpinBox;

    void setupUi(QWidget *Plotter)
    {
        if (Plotter->objectName().isEmpty())
            Plotter->setObjectName(QStringLiteral("Plotter"));
        Plotter->resize(392, 313);
        verticalLayout = new QVBoxLayout(Plotter);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        tabWidget = new QTabWidget(Plotter);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        tab = new QWidget();
        tab->setObjectName(QStringLiteral("tab"));
        verticalLayout_7 = new QVBoxLayout(tab);
        verticalLayout_7->setObjectName(QStringLiteral("verticalLayout_7"));
        plot = new QwtPlot(tab);
        plot->setObjectName(QStringLiteral("plot"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(plot->sizePolicy().hasHeightForWidth());
        plot->setSizePolicy(sizePolicy);

        verticalLayout_7->addWidget(plot);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);

        startStopPlotBtn = new QPushButton(tab);
        startStopPlotBtn->setObjectName(QStringLiteral("startStopPlotBtn"));
        QIcon icon;
        icon.addFile(QStringLiteral(":/images/media-playback-start.png"), QSize(), QIcon::Normal, QIcon::Off);
        icon.addFile(QStringLiteral(":/images/media-playback-stop.png"), QSize(), QIcon::Normal, QIcon::On);
        startStopPlotBtn->setIcon(icon);
        startStopPlotBtn->setCheckable(true);
        startStopPlotBtn->setChecked(true);

        horizontalLayout_2->addWidget(startStopPlotBtn);


        verticalLayout_7->addLayout(horizontalLayout_2);

        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QStringLiteral("tab_2"));
        verticalLayout_3 = new QVBoxLayout(tab_2);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        scrollArea = new QScrollArea(tab_2);
        scrollArea->setObjectName(QStringLiteral("scrollArea"));
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QStringLiteral("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 331, 562));
        verticalLayout_6 = new QVBoxLayout(scrollAreaWidgetContents);
        verticalLayout_6->setObjectName(QStringLiteral("verticalLayout_6"));
        groupBox = new QGroupBox(scrollAreaWidgetContents);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        verticalLayout_2 = new QVBoxLayout(groupBox);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        formLayout = new QFormLayout();
        formLayout->setObjectName(QStringLiteral("formLayout"));
        formLayout->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
        label = new QLabel(groupBox);
        label->setObjectName(QStringLiteral("label"));

        formLayout->setWidget(0, QFormLayout::LabelRole, label);

        idEdit = new QLineEdit(groupBox);
        idEdit->setObjectName(QStringLiteral("idEdit"));

        formLayout->setWidget(0, QFormLayout::FieldRole, idEdit);

        label_2 = new QLabel(groupBox);
        label_2->setObjectName(QStringLiteral("label_2"));

        formLayout->setWidget(4, QFormLayout::LabelRole, label_2);

        typeComboBox = new QComboBox(groupBox);
        typeComboBox->setObjectName(QStringLiteral("typeComboBox"));

        formLayout->setWidget(4, QFormLayout::FieldRole, typeComboBox);

        label_5 = new QLabel(groupBox);
        label_5->setObjectName(QStringLiteral("label_5"));

        formLayout->setWidget(1, QFormLayout::LabelRole, label_5);

        titleEdit = new QLineEdit(groupBox);
        titleEdit->setObjectName(QStringLiteral("titleEdit"));

        formLayout->setWidget(1, QFormLayout::FieldRole, titleEdit);

        label_6 = new QLabel(groupBox);
        label_6->setObjectName(QStringLiteral("label_6"));

        formLayout->setWidget(5, QFormLayout::LabelRole, label_6);

        bufferSpin = new QSpinBox(groupBox);
        bufferSpin->setObjectName(QStringLiteral("bufferSpin"));
        bufferSpin->setMinimum(10);
        bufferSpin->setMaximum(1000);
        bufferSpin->setValue(100);

        formLayout->setWidget(5, QFormLayout::FieldRole, bufferSpin);

        label_7 = new QLabel(groupBox);
        label_7->setObjectName(QStringLiteral("label_7"));

        formLayout->setWidget(6, QFormLayout::LabelRole, label_7);

        refreshRateSpin = new QSpinBox(groupBox);
        refreshRateSpin->setObjectName(QStringLiteral("refreshRateSpin"));
        refreshRateSpin->setMinimum(10);
        refreshRateSpin->setMaximum(10000);
        refreshRateSpin->setValue(50);

        formLayout->setWidget(6, QFormLayout::FieldRole, refreshRateSpin);

        label_9 = new QLabel(groupBox);
        label_9->setObjectName(QStringLiteral("label_9"));

        formLayout->setWidget(2, QFormLayout::LabelRole, label_9);

        xaxisEdit = new QLineEdit(groupBox);
        xaxisEdit->setObjectName(QStringLiteral("xaxisEdit"));

        formLayout->setWidget(2, QFormLayout::FieldRole, xaxisEdit);

        label_10 = new QLabel(groupBox);
        label_10->setObjectName(QStringLiteral("label_10"));

        formLayout->setWidget(3, QFormLayout::LabelRole, label_10);

        yaxisEdit = new QLineEdit(groupBox);
        yaxisEdit->setObjectName(QStringLiteral("yaxisEdit"));

        formLayout->setWidget(3, QFormLayout::FieldRole, yaxisEdit);


        verticalLayout_2->addLayout(formLayout);


        verticalLayout_6->addWidget(groupBox);

        groupBox_2 = new QGroupBox(scrollAreaWidgetContents);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        verticalLayout_4 = new QVBoxLayout(groupBox_2);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        formLayout_2 = new QFormLayout();
        formLayout_2->setObjectName(QStringLiteral("formLayout_2"));
        formLayout_2->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
        autoScaleYCheckBox = new QCheckBox(groupBox_2);
        autoScaleYCheckBox->setObjectName(QStringLiteral("autoScaleYCheckBox"));
        autoScaleYCheckBox->setChecked(true);

        formLayout_2->setWidget(1, QFormLayout::LabelRole, autoScaleYCheckBox);

        label_3 = new QLabel(groupBox_2);
        label_3->setObjectName(QStringLiteral("label_3"));

        formLayout_2->setWidget(2, QFormLayout::LabelRole, label_3);

        minYSpin = new QDoubleSpinBox(groupBox_2);
        minYSpin->setObjectName(QStringLiteral("minYSpin"));
        minYSpin->setMinimum(-1e+07);
        minYSpin->setMaximum(1e+07);
        minYSpin->setValue(-200);

        formLayout_2->setWidget(2, QFormLayout::FieldRole, minYSpin);

        label_4 = new QLabel(groupBox_2);
        label_4->setObjectName(QStringLiteral("label_4"));

        formLayout_2->setWidget(3, QFormLayout::LabelRole, label_4);

        maxYSpin = new QDoubleSpinBox(groupBox_2);
        maxYSpin->setObjectName(QStringLiteral("maxYSpin"));
        maxYSpin->setMinimum(-1e+07);
        maxYSpin->setMaximum(1e+07);
        maxYSpin->setValue(200);

        formLayout_2->setWidget(3, QFormLayout::FieldRole, maxYSpin);

        autoScaleXCheckBox = new QCheckBox(groupBox_2);
        autoScaleXCheckBox->setObjectName(QStringLiteral("autoScaleXCheckBox"));
        autoScaleXCheckBox->setChecked(true);

        formLayout_2->setWidget(4, QFormLayout::LabelRole, autoScaleXCheckBox);

        label_11 = new QLabel(groupBox_2);
        label_11->setObjectName(QStringLiteral("label_11"));

        formLayout_2->setWidget(5, QFormLayout::LabelRole, label_11);

        minXSpin = new QDoubleSpinBox(groupBox_2);
        minXSpin->setObjectName(QStringLiteral("minXSpin"));
        minXSpin->setMinimum(-1e+07);
        minXSpin->setMaximum(1e+07);
        minXSpin->setValue(-200);

        formLayout_2->setWidget(5, QFormLayout::FieldRole, minXSpin);

        maxXSpin = new QDoubleSpinBox(groupBox_2);
        maxXSpin->setObjectName(QStringLiteral("maxXSpin"));
        maxXSpin->setMinimum(-1e+07);
        maxXSpin->setMaximum(1e+07);
        maxXSpin->setValue(200);

        formLayout_2->setWidget(6, QFormLayout::FieldRole, maxXSpin);

        label_12 = new QLabel(groupBox_2);
        label_12->setObjectName(QStringLiteral("label_12"));

        formLayout_2->setWidget(6, QFormLayout::LabelRole, label_12);


        verticalLayout_4->addLayout(formLayout_2);


        verticalLayout_6->addWidget(groupBox_2);

        groupBox_3 = new QGroupBox(scrollAreaWidgetContents);
        groupBox_3->setObjectName(QStringLiteral("groupBox_3"));
        verticalLayout_5 = new QVBoxLayout(groupBox_3);
        verticalLayout_5->setObjectName(QStringLiteral("verticalLayout_5"));
        formLayout_3 = new QFormLayout();
        formLayout_3->setObjectName(QStringLiteral("formLayout_3"));
        label_8 = new QLabel(groupBox_3);
        label_8->setObjectName(QStringLiteral("label_8"));

        formLayout_3->setWidget(0, QFormLayout::LabelRole, label_8);

        dateFormat = new QLineEdit(groupBox_3);
        dateFormat->setObjectName(QStringLiteral("dateFormat"));

        formLayout_3->setWidget(0, QFormLayout::FieldRole, dateFormat);

        fillCheckBox = new QCheckBox(groupBox_3);
        fillCheckBox->setObjectName(QStringLiteral("fillCheckBox"));
        fillCheckBox->setChecked(true);

        formLayout_3->setWidget(1, QFormLayout::LabelRole, fillCheckBox);

        label_13 = new QLabel(groupBox_3);
        label_13->setObjectName(QStringLiteral("label_13"));

        formLayout_3->setWidget(3, QFormLayout::LabelRole, label_13);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);

        colorLbl = new QLabel(groupBox_3);
        colorLbl->setObjectName(QStringLiteral("colorLbl"));
        colorLbl->setMinimumSize(QSize(50, 0));
        colorLbl->setMaximumSize(QSize(50, 16777215));
        colorLbl->setStyleSheet(QLatin1String("background-color:#ffffff;\n"
"border: 2px solid #000;"));

        horizontalLayout->addWidget(colorLbl);

        colorBtn = new QPushButton(groupBox_3);
        colorBtn->setObjectName(QStringLiteral("colorBtn"));
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/images/kcoloredit.png"), QSize(), QIcon::Normal, QIcon::Off);
        colorBtn->setIcon(icon1);

        horizontalLayout->addWidget(colorBtn);


        formLayout_3->setLayout(3, QFormLayout::FieldRole, horizontalLayout);

        label_14 = new QLabel(groupBox_3);
        label_14->setObjectName(QStringLiteral("label_14"));

        formLayout_3->setWidget(2, QFormLayout::LabelRole, label_14);

        transparentSpinBox = new QSpinBox(groupBox_3);
        transparentSpinBox->setObjectName(QStringLiteral("transparentSpinBox"));
        transparentSpinBox->setMinimum(20);
        transparentSpinBox->setMaximum(100);

        formLayout_3->setWidget(2, QFormLayout::FieldRole, transparentSpinBox);


        verticalLayout_5->addLayout(formLayout_3);


        verticalLayout_6->addWidget(groupBox_3);

        scrollArea->setWidget(scrollAreaWidgetContents);

        verticalLayout_3->addWidget(scrollArea);

        tabWidget->addTab(tab_2, QString());

        verticalLayout->addWidget(tabWidget);


        retranslateUi(Plotter);

        tabWidget->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(Plotter);
    } // setupUi

    void retranslateUi(QWidget *Plotter)
    {
        Plotter->setWindowTitle(QApplication::translate("Plotter", "Form", 0));
#ifndef QT_NO_TOOLTIP
        startStopPlotBtn->setToolTip(QApplication::translate("Plotter", "<html><head/><body><p>Starts/stops data collecting from Serial</p><p>When stopped, you'll be able to zoom in/out and move the graph to further inspect the data.</p></body></html>", 0));
#endif // QT_NO_TOOLTIP
        startStopPlotBtn->setText(QString());
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("Plotter", "Plot", 0));
        groupBox->setTitle(QApplication::translate("Plotter", "Basic", 0));
#ifndef QT_NO_TOOLTIP
        label->setToolTip(QApplication::translate("Plotter", "<html><head/><body><p>The ID of the graph, for serial thread to properly forward the incoming data from micro-controller.</p><p>You can have the same IDs if you wish, both will receive the packet from the serial thread.</p></body></html>", 0));
#endif // QT_NO_TOOLTIP
        label->setText(QApplication::translate("Plotter", "ID:", 0));
        idEdit->setText(QApplication::translate("Plotter", "p1", 0));
#ifndef QT_NO_TOOLTIP
        label_2->setToolTip(QApplication::translate("Plotter", "<html><head/><body><p>Can select the graph to plot in real time or XY plot</p><p><span style=\" font-size:10pt;\">Sample real-time packet with 3 curves XYZ:</span></p><p>{<span style=\" color:#ff0000;\">P0|X|255,0,0|100</span><span style=\" color:#00ff7f;\">|Y|0,255,0|150</span><span style=\" color:#5555ff;\">|Z|0,0,255|200</span>}</p><p>{Plot ID|Curve 1|Color|Value|Curve 2|Color|Value}</p><p><span style=\" font-weight:600;\">Note: color is in RGB format with comma seperating the values (R,G,B)</span></p><p><span style=\" font-size:10pt;\">Sample XY plot (supports only one curve):</span></p><p>{P0|Some XY plot|255,0,0|1 255 2 200 3 150 4 100}</p><p>{Plot ID|Curve name|Color|X Y X Y X Y X Y.....}</p><p><span style=\" font-weight:600;\">Note: If for each X value there won't be Y value, the plot will be ignored</span><br/></p></body></html>", 0));
#endif // QT_NO_TOOLTIP
        label_2->setText(QApplication::translate("Plotter", "Type:", 0));
        typeComboBox->clear();
        typeComboBox->insertItems(0, QStringList()
         << QApplication::translate("Plotter", "Time plot", 0)
         << QApplication::translate("Plotter", "XY-Plot", 0)
        );
        label_5->setText(QApplication::translate("Plotter", "Title", 0));
        titleEdit->setText(QApplication::translate("Plotter", "A Simple Plotting Demonstration", 0));
#ifndef QT_NO_TOOLTIP
        label_6->setToolTip(QApplication::translate("Plotter", "<html><head/><body><p>Applicable only to real time plot, basically, how many data points will be stored in the memory before deleting them. </p></body></html>", 0));
#endif // QT_NO_TOOLTIP
        label_6->setText(QApplication::translate("Plotter", "Buffer", 0));
#ifndef QT_NO_TOOLTIP
        label_7->setToolTip(QApplication::translate("Plotter", "<html><head/><body><p>How often the plot will be updated. You shouldn't need less than 50ms, however if you want to improve performance of the application, it is advisable to increase the refresh rate. </p></body></html>", 0));
#endif // QT_NO_TOOLTIP
        label_7->setText(QApplication::translate("Plotter", "Refresh rate (ms)", 0));
        label_9->setText(QApplication::translate("Plotter", "Name of X Axis:", 0));
        label_10->setText(QApplication::translate("Plotter", "Name of Y Axis", 0));
        groupBox_2->setTitle(QApplication::translate("Plotter", "Scaling", 0));
#ifndef QT_NO_TOOLTIP
        autoScaleYCheckBox->setToolTip(QApplication::translate("Plotter", "Automatically scales the graph to show all values on Y axis", 0));
#endif // QT_NO_TOOLTIP
        autoScaleYCheckBox->setText(QApplication::translate("Plotter", "Auto Scaling Y", 0));
        label_3->setText(QApplication::translate("Plotter", "Min Y", 0));
        label_4->setText(QApplication::translate("Plotter", "Max Y", 0));
#ifndef QT_NO_TOOLTIP
        autoScaleXCheckBox->setToolTip(QApplication::translate("Plotter", "Automatically scales graph to include all X values (manual scaling works only on XY plot)", 0));
#endif // QT_NO_TOOLTIP
        autoScaleXCheckBox->setText(QApplication::translate("Plotter", "Auto Scaling X", 0));
        label_11->setText(QApplication::translate("Plotter", "Min X", 0));
        label_12->setText(QApplication::translate("Plotter", "Max X", 0));
        groupBox_3->setTitle(QApplication::translate("Plotter", "Misc", 0));
#ifndef QT_NO_TOOLTIP
        label_8->setToolTip(QApplication::translate("Plotter", "<html><head/><body><p>Format of the date in real time plot. Some of the available formats: </p><p><span style=\" font-weight:600;\">&quot;dd:mm:yyyy | hh:mm:ss:zzz&quot;</span></p><p>Where it shows<span style=\" font-weight:600;\"> day:month:year | hours:minutes:seconds:milliseconds</span></p></body></html>", 0));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_STATUSTIP
        label_8->setStatusTip(QString());
#endif // QT_NO_STATUSTIP
        label_8->setText(QApplication::translate("Plotter", "Date Format", 0));
        dateFormat->setText(QApplication::translate("Plotter", "hh:mm:ss", 0));
#ifndef QT_NO_TOOLTIP
        fillCheckBox->setToolTip(QApplication::translate("Plotter", "Automatically fills the graph", 0));
#endif // QT_NO_TOOLTIP
        fillCheckBox->setText(QApplication::translate("Plotter", "Auto filling", 0));
        label_13->setText(QApplication::translate("Plotter", "Background Color:", 0));
        colorLbl->setText(QString());
#ifndef QT_NO_TOOLTIP
        colorBtn->setToolTip(QApplication::translate("Plotter", "Choose background color", 0));
#endif // QT_NO_TOOLTIP
        colorBtn->setText(QString());
        label_14->setText(QApplication::translate("Plotter", "Auto Fill Opacity:", 0));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("Plotter", "Settings", 0));
    } // retranslateUi

};

namespace Ui {
    class Plotter: public Ui_Plotter {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PLOTTER_H

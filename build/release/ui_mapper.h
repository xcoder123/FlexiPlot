/********************************************************************************
** Form generated from reading UI file 'mapper.ui'
**
** Created by: Qt User Interface Compiler version 5.3.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAPPER_H
#define UI_MAPPER_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "gmapwidget.h"

QT_BEGIN_NAMESPACE

class Ui_Mapper
{
public:
    QVBoxLayout *verticalLayout_2;
    QTabWidget *tabWidget;
    QWidget *tab;
    QVBoxLayout *verticalLayout;
    QLabel *titleLbl;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_15;
    QLineEdit *locEdit;
    GMapWidget *mapWidget;
    QWidget *tab_2;

    void setupUi(QWidget *Mapper)
    {
        if (Mapper->objectName().isEmpty())
            Mapper->setObjectName(QStringLiteral("Mapper"));
        Mapper->resize(461, 336);
        QIcon icon;
        icon.addFile(QStringLiteral(":/images/04_maps.png"), QSize(), QIcon::Normal, QIcon::Off);
        Mapper->setWindowIcon(icon);
        verticalLayout_2 = new QVBoxLayout(Mapper);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        tabWidget = new QTabWidget(Mapper);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        tab = new QWidget();
        tab->setObjectName(QStringLiteral("tab"));
        verticalLayout = new QVBoxLayout(tab);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        titleLbl = new QLabel(tab);
        titleLbl->setObjectName(QStringLiteral("titleLbl"));
        titleLbl->setLineWidth(1);

        verticalLayout->addWidget(titleLbl);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        label_15 = new QLabel(tab);
        label_15->setObjectName(QStringLiteral("label_15"));

        horizontalLayout_4->addWidget(label_15);

        locEdit = new QLineEdit(tab);
        locEdit->setObjectName(QStringLiteral("locEdit"));

        horizontalLayout_4->addWidget(locEdit);


        verticalLayout->addLayout(horizontalLayout_4);

        mapWidget = new GMapWidget(tab);
        mapWidget->setObjectName(QStringLiteral("mapWidget"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(mapWidget->sizePolicy().hasHeightForWidth());
        mapWidget->setSizePolicy(sizePolicy);

        verticalLayout->addWidget(mapWidget);

        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QStringLiteral("tab_2"));
        tabWidget->addTab(tab_2, QString());

        verticalLayout_2->addWidget(tabWidget);


        retranslateUi(Mapper);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(Mapper);
    } // setupUi

    void retranslateUi(QWidget *Mapper)
    {
        Mapper->setWindowTitle(QApplication::translate("Mapper", "Form", 0));
        titleLbl->setText(QApplication::translate("Mapper", "<html><head/><body><p align=\"center\"><span style=\" font-size:11pt; font-weight:600;\">Sample map plot</span></p></body></html>", 0));
        label_15->setText(QApplication::translate("Mapper", "Location:", 0));
        locEdit->setText(QApplication::translate("Mapper", "Riga, Latvia", 0));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("Mapper", "Map", 0));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("Mapper", "Settings", 0));
    } // retranslateUi

};

namespace Ui {
    class Mapper: public Ui_Mapper {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAPPER_H

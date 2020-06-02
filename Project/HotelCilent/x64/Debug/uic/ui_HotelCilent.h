/********************************************************************************
** Form generated from reading UI file 'HotelCilent.ui'
**
** Created by: Qt User Interface Compiler version 5.12.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_HOTELCILENT_H
#define UI_HOTELCILENT_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_HotelCilentClass
{
public:
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QWidget *centralWidget;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *HotelCilentClass)
    {
        if (HotelCilentClass->objectName().isEmpty())
            HotelCilentClass->setObjectName(QString::fromUtf8("HotelCilentClass"));
        HotelCilentClass->resize(600, 400);
        menuBar = new QMenuBar(HotelCilentClass);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        HotelCilentClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(HotelCilentClass);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        HotelCilentClass->addToolBar(mainToolBar);
        centralWidget = new QWidget(HotelCilentClass);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        HotelCilentClass->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(HotelCilentClass);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        HotelCilentClass->setStatusBar(statusBar);

        retranslateUi(HotelCilentClass);

        QMetaObject::connectSlotsByName(HotelCilentClass);
    } // setupUi

    void retranslateUi(QMainWindow *HotelCilentClass)
    {
        HotelCilentClass->setWindowTitle(QApplication::translate("HotelCilentClass", "HotelCilent", nullptr));
    } // retranslateUi

};

namespace Ui {
    class HotelCilentClass: public Ui_HotelCilentClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_HOTELCILENT_H

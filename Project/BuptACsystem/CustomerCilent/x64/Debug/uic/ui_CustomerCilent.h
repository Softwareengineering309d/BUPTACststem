/********************************************************************************
** Form generated from reading UI file 'CustomerCilent.ui'
**
** Created by: Qt User Interface Compiler version 5.12.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CUSTOMERCILENT_H
#define UI_CUSTOMERCILENT_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CustomerCilentClass
{
public:
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QWidget *centralWidget;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *CustomerCilentClass)
    {
        if (CustomerCilentClass->objectName().isEmpty())
            CustomerCilentClass->setObjectName(QString::fromUtf8("CustomerCilentClass"));
        CustomerCilentClass->resize(600, 400);
        menuBar = new QMenuBar(CustomerCilentClass);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        CustomerCilentClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(CustomerCilentClass);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        CustomerCilentClass->addToolBar(mainToolBar);
        centralWidget = new QWidget(CustomerCilentClass);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        CustomerCilentClass->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(CustomerCilentClass);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        CustomerCilentClass->setStatusBar(statusBar);

        retranslateUi(CustomerCilentClass);

        QMetaObject::connectSlotsByName(CustomerCilentClass);
    } // setupUi

    void retranslateUi(QMainWindow *CustomerCilentClass)
    {
        CustomerCilentClass->setWindowTitle(QApplication::translate("CustomerCilentClass", "CustomerCilent", nullptr));
    } // retranslateUi

};

namespace Ui {
    class CustomerCilentClass: public Ui_CustomerCilentClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CUSTOMERCILENT_H

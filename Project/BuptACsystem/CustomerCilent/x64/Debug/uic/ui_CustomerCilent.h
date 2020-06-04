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
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CustomerCilentClass
{
public:
    QWidget *centralWidget;
    QPushButton *pushButton;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *CustomerCilentClass)
    {
        if (CustomerCilentClass->objectName().isEmpty())
            CustomerCilentClass->setObjectName(QString::fromUtf8("CustomerCilentClass"));
        CustomerCilentClass->resize(600, 400);
        centralWidget = new QWidget(CustomerCilentClass);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        pushButton = new QPushButton(centralWidget);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(220, 160, 75, 23));
        CustomerCilentClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(CustomerCilentClass);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 600, 23));
        CustomerCilentClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(CustomerCilentClass);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        CustomerCilentClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(CustomerCilentClass);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        CustomerCilentClass->setStatusBar(statusBar);

        retranslateUi(CustomerCilentClass);

        QMetaObject::connectSlotsByName(CustomerCilentClass);
    } // setupUi

    void retranslateUi(QMainWindow *CustomerCilentClass)
    {
        CustomerCilentClass->setWindowTitle(QApplication::translate("CustomerCilentClass", "CustomerCilent", nullptr));
        pushButton->setText(QApplication::translate("CustomerCilentClass", "text", nullptr));
    } // retranslateUi

};

namespace Ui {
    class CustomerCilentClass: public Ui_CustomerCilentClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CUSTOMERCILENT_H

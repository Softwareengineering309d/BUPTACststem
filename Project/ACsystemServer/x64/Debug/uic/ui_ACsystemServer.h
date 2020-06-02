/********************************************************************************
** Form generated from reading UI file 'ACsystemServer.ui'
**
** Created by: Qt User Interface Compiler version 5.12.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ACSYSTEMSERVER_H
#define UI_ACSYSTEMSERVER_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ACsystemServerClass
{
public:
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QWidget *centralWidget;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *ACsystemServerClass)
    {
        if (ACsystemServerClass->objectName().isEmpty())
            ACsystemServerClass->setObjectName(QString::fromUtf8("ACsystemServerClass"));
        ACsystemServerClass->resize(600, 400);
        menuBar = new QMenuBar(ACsystemServerClass);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        ACsystemServerClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(ACsystemServerClass);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        ACsystemServerClass->addToolBar(mainToolBar);
        centralWidget = new QWidget(ACsystemServerClass);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        ACsystemServerClass->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(ACsystemServerClass);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        ACsystemServerClass->setStatusBar(statusBar);

        retranslateUi(ACsystemServerClass);

        QMetaObject::connectSlotsByName(ACsystemServerClass);
    } // setupUi

    void retranslateUi(QMainWindow *ACsystemServerClass)
    {
        ACsystemServerClass->setWindowTitle(QApplication::translate("ACsystemServerClass", "ACsystemServer", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ACsystemServerClass: public Ui_ACsystemServerClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ACSYSTEMSERVER_H

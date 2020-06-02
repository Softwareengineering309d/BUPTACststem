#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_ACsystemServer.h"

class ACsystemServer : public QMainWindow
{
    Q_OBJECT

public:
    ACsystemServer(QWidget *parent = Q_NULLPTR);

private:
    Ui::ACsystemServerClass ui;
};

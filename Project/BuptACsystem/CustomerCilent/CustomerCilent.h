#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_CustomerCilent.h"
#include<qpushbutton.h>
#pragma execution_character_set("utf-8")

class CustomerCilent : public QMainWindow
{
    Q_OBJECT

public:
    CustomerCilent(QWidget *parent = Q_NULLPTR);

private:
    Ui::CustomerCilentClass ui;
};

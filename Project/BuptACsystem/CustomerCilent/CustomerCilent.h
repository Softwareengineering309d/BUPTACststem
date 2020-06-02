#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_CustomerCilent.h"

class CustomerCilent : public QMainWindow
{
    Q_OBJECT

public:
    CustomerCilent(QWidget *parent = Q_NULLPTR);

private:
    Ui::CustomerCilentClass ui;
};

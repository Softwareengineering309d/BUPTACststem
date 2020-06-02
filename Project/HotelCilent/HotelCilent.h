#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_HotelCilent.h"

class HotelCilent : public QMainWindow
{
    Q_OBJECT

public:
    HotelCilent(QWidget *parent = Q_NULLPTR);

private:
    Ui::HotelCilentClass ui;
};

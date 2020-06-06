#pragma once

#include <QtWidgets/QMainWindow>
#include <QtNetwork/QTcpsocket.h>
#include "ui_CustomerCilent.h"
#include "ClientTcpSocket.h"
#include <qpushbutton.h>
#pragma execution_character_set("utf-8")

class CustomerCilent : public QMainWindow
{
    Q_OBJECT

public:
    CustomerCilent(QWidget *parent = Q_NULLPTR);
    ClientTcpSocket* socket;
private slots:
    void socketReadData();
    void socketDisconnect();

private:
    Ui::CustomerCilentClass ui;
    QString serverIP = "127.0.0.1";
    quint16 serverPort = 23333;
};

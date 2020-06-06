#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_ACsystemServer.h"
#include "TcpSocket.h"
#include "TcpServer.h"

class ACsystemServer : public QMainWindow
{
    Q_OBJECT

public:
    ACsystemServer(QWidget *parent = Q_NULLPTR);


private slots:
    void receiveData(QByteArray data);
//    void serverNewConnect();
//    void socketReadData();
//    void socketDisconnected();

private:
    Ui::ACsystemServerClass ui;
    TcpServer* server;
    quint16 port = 23333;
};

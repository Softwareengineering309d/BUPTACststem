#pragma once

#include <QtWidgets/QMainWindow>
#include <qvector.h>
#include "ui_ACsystemServer.h"
#include "TcpSocket.h"
#include "TcpServer.h"
#include "Scheduler.h"
#include "ACController.h"

class Scheduler;
class ACController;

class ACsystemServer : public QMainWindow
{
    Q_OBJECT

public:
    ACsystemServer(QWidget *parent = Q_NULLPTR);

    static Scheduler scheduler;
    static ACController acController;
    static QVector<int> waitingQueue;
    static QVector<int> serviceQueue;

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

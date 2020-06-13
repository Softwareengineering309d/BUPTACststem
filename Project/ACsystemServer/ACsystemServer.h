#pragma once

#include <qobject.h>
#include <qvector.h>
#include "TcpSocket.h"
#include "TcpServer.h"
#include "Scheduler.h"
#include "ACController.h"
#include"DBFacade.h"


class ACsystemServer : public QObject
{
    Q_OBJECT

public:
    ACsystemServer(QObject* parent);
    ACsystemServer() :scheduler(this),acController(this),dbfacade(this){
        server = new TcpServer(this, 23333);
        ConnectServerScheduler();
    };
    Scheduler scheduler;
    ACController acController;
    DBFacade dbfacade;
    void ConnectServerScheduler();

private slots:
    void receiveData(QByteArray data);
//    void serverNewConnect();
//    void socketReadData();
//    void socketDisconnected();

private:
    TcpServer* server;
    quint16 port = 23333;
};

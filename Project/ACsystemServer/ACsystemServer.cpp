#include "ACsystemServer.h"
#include <QDebug.h>

ACsystemServer::ACsystemServer(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    server = new TcpServer(this, 23333);


    /**********************************
    * TcpServer使用说明： 李卓
    * 1. 从客户端收到消息：
    * 从客户端发来的包已经从收到的json包转为了消息，把消息绑定在处理该消息的槽函数上即可。
    * 示例：  
    *   connect(server, TcpServer::turnOnAirConditioner, this, [=](int roomID) {
    *      //收到开启空调请求的处理
    *   });
    * 2. 往客户端发消息：
    * 往客户端发消息时直接调用server的几个public函数即可。
    * 示例：
    *   想要向客户端发送已到达指定温度时调用下列函数即可：
    *       server->reachTargetTempStop(roomID);
    **********************************/

    ////用来测试解析是否成功
    //connect(server, &TcpServer::changeFanSpeed, this, [=](int roomID, int fanSpeed) {
    //    qDebug() << "roomID: " << roomID << "\nfanspeed" << fanSpeed;
    //});

    ////测试消息是否触发
    //connect(server, &TcpServer::turnOnAirConditioner, this, [=](int roomID) {
    //    qDebug() << "Turn on conditioner Successful";
    //    server->turnOnAirConditionerBack(roomID, 24, 3);
    //    server->changeTempBack(roomID, true);
    //    server->changeFanSpeedBack(roomID, true);

    //    Sleep(10000);

    //    server->turnOffAirConditionerBack(roomID, true);
    //    server->serviceOn(roomID, 111);
    //    server->reachTargetTempStop(roomID);
    //    server->preemptedStop(roomID, 111, 222);
    //});
}

void ACsystemServer::receiveData(QByteArray data) {
    QString str;
    if (!data.isEmpty()) {
        str += tr(data);
    }
    qDebug() << str;
}

////单对单的demo
//void ACsystemServer::serverNewConnect() {
//    TcpSocket* newClient = new TcpSocket(this);
//    QTcpSocket* socket;
//    socket = server->nextPendingConnection();
//    connect(socket, &QTcpSocket::readyRead, this, &ACsystemServer::socketReadData);
//    connect(socket, &QTcpSocket::disconnected, this, &ACsystemServer::socketDisconnected);
//
//    qDebug() << "Client connected";
//
//    socket->write("Hi");
//}
//
//void ACsystemServer::socketReadData() {
//
//    QByteArray buffer;
//
//    buffer = socket->readAll();
//    QString str;
//    if (!buffer.isEmpty()) {
//        str += tr(buffer);
//    }
//    qDebug() << "buffer: " << str;
//}
//
//void ACsystemServer::socketDisconnected() {
//    qDebug() << "Client disconnected";
//}

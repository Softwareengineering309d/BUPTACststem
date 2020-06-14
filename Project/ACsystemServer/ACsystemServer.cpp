#include "ACsystemServer.h"
#include"global.h"
#include <QDebug.h>




ACsystemServer::ACsystemServer(QObject* parent)
    :QObject(parent),scheduler(this),acController(this),dbfacade(this)
{
    server = new TcpServer(this, 23333);
    ConnectServerScheduler();
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
    //    server->turnOnAirConditionerBack(roomID, 24, 3, 1);
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

void ACsystemServer::ConnectServerScheduler()
{
    /*
    完成对收到信息的处理
    */
    //收到客户端的开机请求
    connect(server, &TcpServer::turnOnAirConditioner, this, [=](int roomID) {
        scheduler.requestOn(roomID);
    });
    //收到客户端的修改温度请求
    connect(server, &TcpServer::changeTemp, this, [=](int roomID, float TargetTemp) {
        scheduler.changetargetTemp(roomID, TargetTemp);
    });
    //收到客户端修改风速的请求
    connect(server, &TcpServer::changeFanSpeed, this, [=](int roomID, int FanSpeed) {
        scheduler.changeFanSpeed(roomID, FanSpeed);
    });
    //收到客户端的关机请求
    connect(server, &TcpServer::turnOffAirConditioner, this, [=](int roomID) {
        scheduler.requestOff(roomID);
    });
    //通知客户端服务启动后，客户端回成功，并且附带当前温度
    connect(server, &TcpServer::serviceOnOK, this, [=](int roomID, float CurrentTemp) {
        scheduler.changeCurrentTemp(roomID, CurrentTemp);
    });
    /*
    待完成收到客户心跳包的操作
    */
    //收到客户的心跳包，客户达到目标温度后启动回温程序回心跳包。
    //connect(server, &TcpServer::heartBeat, this, [=](float CurrentTemp) {
        //scheduler.changeCurrentTemp()
    //});
    // 空调开启OK（返回默认参数）
    connect(&scheduler, &Scheduler::turnonOK, server, [=](int roomID, float defaultTemp, int defaultFanSpeed,int Mode, bool succeed = true) {
        server->turnOnAirConditionerBack(roomID, defaultTemp, Mode,defaultFanSpeed, succeed);
    });
    //关机成功，给客户回包
    connect(&scheduler, &Scheduler::turnOffOK, server, [=](int roomID, bool succeed) {

        server->turnOffAirConditionerBack(roomID, succeed);
    });
    //修改温度成功，给客户回包
    connect(&scheduler, &Scheduler::changeTempBack, server, [=](int roomID, bool succeed) {
        server->changeTempBack(roomID, succeed);
    });
    //修改风速成功，给客户回包
    connect(&scheduler, &Scheduler::changeFanSpeedBack, server, [=](int roomID, bool succeed) {
        server->changeFanSpeedBack(roomID, succeed);
    });
    //服务启动，给客户回包
    connect(&scheduler, &Scheduler::serviceStart, server, [=](int roomID, int serverID) {
        server->serviceOn(roomID, serverID);
    });
    //进入等待状态，给客户回包
    connect(&scheduler, &Scheduler::waitStart, server, [=](int roomID, int waitID, int waitTime) {
        server->preemptedStop(roomID, waitID, waitTime);
    });
    //服务完成，给客户回包
    connect(&scheduler, &Scheduler::serviceFinish, server, [=](int roomID) {
        server->reachTargetTempStop(roomID);
    });
    //给客户端回心跳包
    connect(RoomUp, &RoomUpdate::signal_update_timing, server, [=](int RoomId, int state, float cur_temp, float dist_temp, int speed, float fee) {
        server->keepAlive(RoomId, fee, cur_temp);
    });
}

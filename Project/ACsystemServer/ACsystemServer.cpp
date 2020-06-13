#include "ACsystemServer.h"
#include"global.h"
#include <QDebug.h>




ACsystemServer::ACsystemServer(QObject* parent)
    :QObject(parent),scheduler(this),acController(this),dbfacade(this)
{
    server = new TcpServer(this, 23333);
    ConnectServerScheduler();
    /**********************************
    * TcpServerʹ��˵���� ��׿
    * 1. �ӿͻ����յ���Ϣ��
    * �ӿͻ��˷����İ��Ѿ����յ���json��תΪ����Ϣ������Ϣ���ڴ������Ϣ�Ĳۺ����ϼ��ɡ�
    * ʾ����  
    *   connect(server, TcpServer::turnOnAirConditioner, this, [=](int roomID) {
    *      //�յ������յ�����Ĵ���
    *   });
    * 2. ���ͻ��˷���Ϣ��
    * ���ͻ��˷���Ϣʱֱ�ӵ���server�ļ���public�������ɡ�
    * ʾ����
    *   ��Ҫ��ͻ��˷����ѵ���ָ���¶�ʱ�������к������ɣ�
    *       server->reachTargetTempStop(roomID);
    **********************************/

    ////�������Խ����Ƿ�ɹ�
    //connect(server, &TcpServer::changeFanSpeed, this, [=](int roomID, int fanSpeed) {
    //    qDebug() << "roomID: " << roomID << "\nfanspeed" << fanSpeed;
    //});

    ////������Ϣ�Ƿ񴥷�
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

////���Ե���demo
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
    ��ɶ��յ���Ϣ�Ĵ���
    */
    //�յ��ͻ��˵Ŀ�������
    connect(server, &TcpServer::turnOnAirConditioner, this, [=](int roomID) {
        scheduler.requestOn(roomID);
    });
    //�յ��ͻ��˵��޸��¶�����
    connect(server, &TcpServer::changeTemp, this, [=](int roomID, float TargetTemp) {
        scheduler.changetargetTemp(roomID, TargetTemp);
    });
    //�յ��ͻ����޸ķ��ٵ�����
    connect(server, &TcpServer::changeFanSpeed, this, [=](int roomID, int FanSpeed) {
        scheduler.changeFanSpeed(roomID, FanSpeed);
    });
    //�յ��ͻ��˵Ĺػ�����
    connect(server, &TcpServer::turnOffAirConditioner, this, [=](int roomID) {
        scheduler.requestOff(roomID);
    });
    //֪ͨ�ͻ��˷��������󣬿ͻ��˻سɹ������Ҹ�����ǰ�¶�
    connect(server, &TcpServer::serviceOnOK, this, [=](int roomID, float CurrentTemp) {
        scheduler.changeCurrentTemp(roomID, CurrentTemp);
    });
    /*
    ������յ��ͻ��������Ĳ���
    */
    //�յ��ͻ������������ͻ��ﵽĿ���¶Ⱥ��������³������������
    //connect(server, &TcpServer::heartBeat, this, [=](float CurrentTemp) {
        //scheduler.changeCurrentTemp()
    //});
    // �յ�����OK������Ĭ�ϲ�����
    connect(&scheduler, &Scheduler::turnonOK, server, [=](int roomID, float defaultTemp, int defaultFanSpeed,int Mode, bool succeed = true) {
        server->turnOnAirConditionerBack(roomID, defaultTemp, Mode,defaultFanSpeed, succeed);
    });
    //�ػ��ɹ������ͻ��ذ�
    connect(&scheduler, &Scheduler::turnOffOK, server, [=](int roomID, bool succeed) {

        server->turnOffAirConditionerBack(roomID, succeed);
    });
    //�޸��¶ȳɹ������ͻ��ذ�
    connect(&scheduler, &Scheduler::changeTempBack, server, [=](int roomID, bool succeed) {
        server->changeTempBack(roomID, succeed);
    });
    //�޸ķ��ٳɹ������ͻ��ذ�
    connect(&scheduler, &Scheduler::changeFanSpeedBack, server, [=](int roomID, bool succeed) {
        server->changeFanSpeedBack(roomID, succeed);
    });
    //�������������ͻ��ذ�
    connect(&scheduler, &Scheduler::serviceStart, server, [=](int roomID, int serverID) {
        server->serviceOn(roomID, serverID);
    });
    //����ȴ�״̬�����ͻ��ذ�
    connect(&scheduler, &Scheduler::waitStart, server, [=](int roomID, int waitID, int waitTime) {
        server->preemptedStop(roomID, waitID, waitTime);
    });
    //������ɣ����ͻ��ذ�
    connect(&scheduler, &Scheduler::serviceFinish, server, [=](int roomID) {
        server->reachTargetTempStop(roomID);
    });
    //���ͻ��˻�������
    connect(RoomUp, &RoomUpdate::signal_update_timing, server, [=](int RoomId, int state, float cur_temp, float dist_temp, int speed, float fee) {
        server->keepAlive(RoomId, fee, cur_temp);
    });
}

/************************************************************************
 * FILE				: TcpServer.h
 * AUTHOR			: Li Zhuo
 * DATE				: June 5, 2020
 * CHANGE HISTORY	:
 * Author       Date            Description of change
 * Li Zhuo		June 6, 2020		Add <Windows.h>
 ************************************************************************/
#pragma once

#include <QtNetwork/QTcpServer.h>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QDebug>
#include <QJsonArray>
#include <Qmap.h>
#include <Windows.h>
#include "TcpSocket.h"

class TcpServer : public QTcpServer
{
	Q_OBJECT

public:
	TcpServer(QObject *parent, quint16 port = 23333);
	~TcpServer();
	//������Ӧ�� �յ�����
	void turnOnAirConditionerBack(int roomID, float defaultTemp, int defaultFanSpeed, bool mode, bool succeed = true);
	void turnOnAirConditionerBack(int roomID, bool succeed = false);
	//������Ӧ�� �޸��¶�
	void changeTempBack(int roomID, bool succeed);
	//������Ӧ�� �޸ķ���
	void changeFanSpeedBack(int roomID, bool succeed);
	//������Ӧ�� �ر�
	void turnOffAirConditionerBack(int roomID, bool succeed);
	//������֪ͨ ֪ͨ�ͻ��˷����ѱ����� 
	void serviceOn(int roomID, int serverID);
	//������֪ͨ ֪ͨ�ͻ��˷����ѱ�ֹͣ ����ָ���¶�ֹͣ
	void reachTargetTempStop(int roomID);
	//������֪ͨ ֪ͨ�ͻ��˷����ѱ�ֹͣ ����ռֹͣ
	void preemptedStop(int roomID, int waitID, int waitTime);
	//������
	void keepAlive(int roomID, float totalFee, float CurrentTemp);

	QList<TcpSocket*> clientSocketList;

protected:
	void incomingConnection(qintptr socketDescriptor);

private:
	//�ҵ���Ӧ��socket
	TcpSocket* findSocketByRoomID(int roomID);

	QMap<int, qintptr> roomIDToSocketDescriptor;
	QMap<qintptr, int> socketDescriptorToRoomID;

private slots:
	void receiveData(QByteArray data, qintptr socketDescriptor);
	void clientDisconnected(qintptr socketDescriptor);

signals:
	//�յ����� RoomId
	void turnOnAirConditioner(int);
	//�޸��¶� RoomId, TargetTemp
	void changeTemp(int, float);
	//�޸ķ��� RoomId, FanSpeed
	void changeFanSpeed(int, int);
	//�ر� RoomId
	void turnOffAirConditioner(int);
	//֪ͨ�ͻ��˷����ѱ�����-OK����Ҫ�ͻ��˷��ص�ǰ�¶ȣ�RoomId, Temp
	void serviceOnOK(int, float);
	//֪ͨ�ͻ��˷����ѱ�����-ERROR����ʱ�Ͳ����ص�ǰ�¶��ˣ�RoomId
	void serviceOnError(int);
	//����ָ���¶�ֹͣ-OK RoomId
	void reachTargetTempStopOK(int);
	//����ָ���¶�ֹͣ-Error RoomId
	void reachTargetTempStopError(int);
	//����ռֹͣ-OK RoomId CurrentTemp
	void preemptedStopOK(int, float);
	//����ռֹͣ-Error RoomId
	void preemptedStopError(int);
	//�ͻ�������
	void heartBeat(float);
	//���ڲ���
	void receiveDataRaw(QByteArray);
};

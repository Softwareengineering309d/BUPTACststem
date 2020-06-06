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
	//服务器应答 空调开启
	void turnOnAirConditionerBack(int roomID, float defaultTemp, int defaultFanSpeed, bool succeed = true);
	void turnOnAirConditionerBack(int roomID, bool succeed = false);
	//服务器应答 修改温度
	void changeTempBack(int roomID, bool succeed);
	//服务器应答 修改风速
	void changeFanSpeedBack(int roomID, bool succeed);
	//服务器应答 关闭
	void turnOffAirConditionerBack(int roomID, bool succeed);
	//服务器通知 通知客户端服务已被启动 
	void serviceOn(int roomID, int serverID);
	//服务器通知 通知客户端服务已被停止 到达指定温度停止
	void reachTargetTempStop(int roomID);
	//服务器通知 通知客户端服务已被停止 被抢占停止
	void preemptedStop(int roomID, int waitID, int waitTime);
	//心跳包
	void keepAlive(int roomID, int totalFee, float CurrentTemp);

	QList<TcpSocket*> clientSocketList;

protected:
	void incomingConnection(qintptr socketDescriptor);

private:
	//找到对应的socket
	TcpSocket* findSocketByRoomID(int roomID);

	QMap<int, qintptr> roomIDToSocketDescriptor;
	QMap<qintptr, int> socketDescriptorToRoomID;

private slots:
	void receiveData(QByteArray data, qintptr socketDescriptor);
	void clientDisconnected(qintptr socketDescriptor);

signals:
	//空调开启 RoomId
	void turnOnAirConditioner(int);
	//修改温度 RoomId, TargetTemp
	void changeTemp(int, float);
	//修改风速 RoomId, FanSpeed
	void changeFanSpeed(int, int);
	//关闭 RoomId
	void turnOffAirConditioner(int);
	//通知客户端服务已被启动-OK（需要客户端返回当前温度）RoomId, Temp
	void serviceOnOK(int, float);
	//通知客户端服务已被启动-ERROR（此时就不返回当前温度了）RoomId
	void serviceOnError(int);
	//到达指定温度停止-OK RoomId
	void reachTargetTempStopOK(int);
	//到达指定温度停止-Error RoomId
	void reachTargetTempStopError(int);
	//被抢占停止-OK RoomId
	void preemptedStopOK(int);
	//被抢占停止-Error RoomId
	void preemptedStopError(int);

	//用于测试
	void receiveDataRaw(QByteArray);
};

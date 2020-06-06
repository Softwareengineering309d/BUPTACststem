/************************************************************************
 * FILE				: ClientTcpSocket.h
 * AUTHOR			: Li Zhuo
 * DATE				: June 5, 2020
 * CHANGE HISTORY	:
 * Author       Date            Description of change
 * Li Zhuo		June 6, 2020		Add comments
 ************************************************************************/
#pragma once

#include <QTcpSocket>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QDebug>
#include <QJsonArray>
#include <Windows.h>

class ClientTcpSocket : public QTcpSocket
{
	Q_OBJECT

public:
	ClientTcpSocket(QObject *parent);
	~ClientTcpSocket();
	//客户端请求 空调开启
	void turnOnAirConditioner(int roomID);
	//客户端请求 修改温度
	void changeTemp(int roomID, float targetTemp);
	//客户端请求 修改风速
	void changeFanSpeed(int roomID, int fanSpeed);
	//客户端请求 关闭
	void closeAirConditioner(int roomID);

	//客户端应答 通知客户端服务已被启动
	void serviceOnBack(int roomID, float currentTemp, bool succeed = true);
	void serviceOnBack(int roomID, bool succeed = false);
	//客户端应答 到达指定温度停止
	void reachTargetTempStopBack(bool succeed);
	//客户端应答 被抢占停止
	void preemptedStopBack(bool succeed);
private slots:
	void receiveData();
	void serverDisconnected();

signals:
	//空调开启OK（返回默认参数） DefaultTemp DefaultFanSpeed
	void turnOnAirConditionerOK(float, int);
	//空调开启ERROR
	void turnOnAirConditionerError();
	//修改温度OK
	void changeTempOK();
	//修改温度Error
	void changeTempError();
	//修改风速OK
	void changeFanSpeedOK();
	//修改风速Error
	void changeFanSpeedError();
	//关闭空调OK
	void closeAirConditionerOK();
	//关闭空调Error
	void closeAirConditionerError();

	//服务器通知
	//通知客户端服务已被启动（需要客户端返回当前温度）ServerId ???
	void serviceOn(int);
	//通知客户端服务已被停止 
	//到达指定温度停止（服务已经完成了)
	void reachTargetTempStop();
	//被抢占停止（服务暂时终止了，因此要回送wait号） WaitId WaitTime
	void preemptedStop(int, int);
	//000心跳包，告知客户端当前的温度和费用，直到服务暂停或停止 TotalFee CurrentTemp
	void heartBeat(int, float);
};

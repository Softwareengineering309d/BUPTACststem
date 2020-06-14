/************************************************************************
 * FILE				: ClientTcpSocket.h
 * AUTHOR			: Li Zhuo
 * DATE				: June 5, 2020
 * CHANGE HISTORY	:
 * Author       Date            Description of change
 * Li Zhuo		June 6, 2020		Add comments
 ************************************************************************/
#pragma once

#include <QtNetwork/qtcpsocket.h>
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
	//�ͻ������� �յ�����
	void turnOnAirConditioner(int roomID);
	//�ͻ������� �޸��¶�
	void changeTemp(int roomID, float targetTemp);
	//�ͻ������� �޸ķ���
	void changeFanSpeed(int roomID, int fanSpeed);
	//�ͻ������� �ر�
	void closeAirConditioner(int roomID);

	//�ͻ���Ӧ�� ֪ͨ�ͻ��˷����ѱ�����
	void serviceOnBack(int roomID, float currentTemp, bool succeed = true);
	void serviceOnBack(int roomID, bool succeed = false);
	//�ͻ���Ӧ�� ����ָ���¶�ֹͣ
	void reachTargetTempStopBack(bool succeed);
	//�ͻ���Ӧ�� ����ռֹͣ
	void preemptedStopBack(float currentTemp, bool succeed = true);
	void preemptedStopBack(bool succeed = false);
	//�ͻ���������
	void keepAlive(float curretnTemp);
private slots:
	void receiveData();
	void serverDisconnected();

signals:
	//�յ�����OK������Ĭ�ϲ����� DefaultTemp DefaultFanSpeed Mode
	void turnOnAirConditionerOK(float, int, bool);//0���� 1����
	//�յ�����ERROR
	void turnOnAirConditionerError();
	//�޸��¶�OK
	void changeTempOK();
	//�޸��¶�Error
	void changeTempError();
	//�޸ķ���OK
	void changeFanSpeedOK();
	//�޸ķ���Error
	void changeFanSpeedError();
	//�رտյ�OK
	void closeAirConditionerOK();
	//�رտյ�Error
	void closeAirConditionerError();

	//������֪ͨ
	//֪ͨ�ͻ��˷����ѱ���������Ҫ�ͻ��˷��ص�ǰ�¶ȣ�ServerId ???
	void serviceOn(int);
	//֪ͨ�ͻ��˷����ѱ�ֹͣ 
	//����ָ���¶�ֹͣ�������Ѿ������)
	void reachTargetTempStop();
	//����ռֹͣ��������ʱ��ֹ�ˣ����Ҫ����wait�ţ� WaitId WaitTime
	void preemptedStop(int, int);
	//000����������֪�ͻ��˵�ǰ���¶Ⱥͷ��ã�ֱ��������ͣ��ֹͣ TotalFee CurrentTemp
	void heartBeat(float, float);

};

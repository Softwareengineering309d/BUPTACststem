#pragma once

#include <QObject>
#include"Scheduler.h"


class ACController : public QObject
{
	Q_OBJECT

public:
	ACController(QObject *parent);
	~ACController();
	void connect_with_manager();
	//�ۺ��������������ť���ߴ򿪳���ʱ����
	void powerOn(Scheduler* scheduler);
	//�ۺ���������Ա�����������ʱ���ã����Ͳ��������ȶ���
	void sendPara(Scheduler* scheduler,int defaultMode, float tempHighLimit, float tempLowLimit, float defaultTargetTemp, int FanSpeed,float defaultFeeRate);
	//�ۺ���������Ա�����ʼ����ʱ����
	void startUp(Scheduler* scheduler);
	//ÿ��һ��ʱ�����һ�Σ���ȡ���䵱ǰ״̬��չʾ�ڽ�����
	QVector<int> checkRoomState(Scheduler* scheduler);  //@ע��Ӧ��ΪQVector<RoomBean>����Room����֮����һ���洢�з�����Ϣ�Ķ���

private:
	//��powerOn()�������ã��������ȶ���
	int createScheduler(Scheduler* scheduler);
};

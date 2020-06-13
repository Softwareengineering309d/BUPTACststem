#pragma once
#include"queueRequestObject1.h"
#include"queueServiceObject1.h"
#include <QObject>
#include<qtimer.h>

class Scheduler : public QObject
{
	Q_OBJECT
private:
	int defaultMode;
	float defaultTemp;
	int defaultFanSpeed;
	float hightlimitTemp;
	float lowlimitTemp;
	float FeeRate;
public:
	Scheduler(QObject *parent);
	~Scheduler();
	QTimer *timer;
	queueRequestObject WaitQueue;
	queueServiceObject ServiceQueue;
	//���ÿ�������
	void setParm(int mode, float temp, int speed, float htemp, float ltemp,float feerate);
	void ready();
	//��ø����䵱ǰ״̬
	QVector<int> getRoomState();  //@ע������ֵӦΪ������Ϣ��QVector����д����void���ǵ�Ҫ��

	//�����ȴ����У��������ȶ���ʱ����
	void createWaitingQueue();
	//����������У��������ȶ���ʱ����
	void createServiceQueue();

	//����ֵ����־���ȶ����Ƿ��Ѿ���
	bool isReady;
	//���ܵ��ͻ���������
	void requestOn(int RoomID);

	void requestOff(int RoomID);

	bool changetargetTemp(int RoomID, float targetTemp);

	void changeFanSpeed(int RoomID, int FanSpeed);
	//�����ݿ���д��������
	void changeCurrentTemp(int RoomID, float CurrentTemp);
	/*
	�ۺ���:endwork
	����ɣ� 2.������Ϣд�����ݿ�
	*/
	void endwork(int ServeID);
	/*
	�ۺ�����endwait
	����ɣ� 2. ������Ϣд�����ݿ�
	*/
	void endwait(int WaitID);
signals:
	void turnonOK(int, float, int, int,bool);
	void turnonEROOR(int, bool);
	void turnOffOK(int,bool);
	void changeTempBack(int, bool);
	void changeFanSpeedBack(int, bool);
	void serviceStart(int, int);
	void serviceFinish(int);
	void waitStart(int, int, int);
	void keepAlive(int, int , float);
	/*
	todo: ���ݿ�������ź�
	*/
};

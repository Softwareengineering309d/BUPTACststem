#pragma once
#include <QObject>
#include<ctime>
#include<vector>
#include<qdebug.h>
#include<qtimer.h>
#define LOW_FANSPEED 0
#define MID_FANSPEED 1
#define HIGH_FANSPEED 2
//��������
#define REFRIGERATION 0
#define HEATING 1
//����״̬
#define WORKING 0
#define IDLE 1
#define INTERVAL 10000
#define FEERATE 1
class RequestObject
{
private:
	int WaitID;
	int RoomID;
	float currentemp;
	float TargetTemp;
	/*
	���������ֱ��Ϊ
	#define LOW_FANSPEED 3
    #define MID_FANSPEED 2
    #define HIGH_FANSPEED 1
	*/
	int FanSeed;
	/*
	��������
	#define REFRIGERATION 1
	#define HEATING 2
	*/
	int Mode;
	//ĿǰΪֹ�����������ܷ���
	float Fee;
	//ʱ��Ƭ
	int TimeSlice;

public:
	RequestObject();
	RequestObject(float defaultTemp, float defaultSpeed, int defaultmode);
	~RequestObject();
	void assignWait(int waitID);
	void startWait(int RoomID,float ctemp,float ttemp,int speed,float Fee,int Mode);
	//get��ȡ��Ϣ�ĺ���
	float getFee();
	int getRoomID();
	int getWaitID();
	float getCurrenTemp();
	float getTargetTemp();
	int getFanSpeed();
	int getTimeSlice();
	int getMode();
	//�޸�Ŀ���¶Ⱥͷ���
	void modifyTtemp(float ttemp);
	void modifyFanSpeed(float speed);
	//ʱ��Ƭ����
	void modifyTimeSlice(int newtimeslice);
	//���µ�ǰ����
	void updatectemp(float speed);
};

class queueRequestObject : public QObject
{
	Q_OBJECT

public:
	queueRequestObject(QObject *parent);
	~queueRequestObject();
	void addRequestObject(RequestObject* ro);
	//ÿ10sˢ��һ�ε�ǰ��������������ʱ��Ƭ����
	void refreshTimeSlice();
	//���ݷ���ţ��ȴ�ID�����ʱ��Ƭ(���ߵ����ȼ�)�ҵ���Ӧ�������ĵ�ַ
	RequestObject* getRequestObjectRoomID(int RoomID);
	RequestObject* getRequestObjectWaitID(int WaitID);
	RequestObject* getRequestObjectShortest();
	RequestObject* getRequestObjectShortest(int FanSpeed);
	void deleteRequest(int WaitID);
	int size();
	int getawaitid();
	bool empty();
protected:
	QVector<RequestObject*> queue;
signals:
	void waitFinish(int);
};

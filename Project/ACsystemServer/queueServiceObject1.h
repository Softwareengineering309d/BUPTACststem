#pragma once

#include <QObject>
#include<qvector.h>
#include<qtimer.h>
#include<qdebug.h>
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
class RequestObject;
class queueRequestObject;
class ServiceObject
{
private:
	int ServerID;
	int RoomID;
	/*
	����״̬
    #define WORKING 0
    #define IDLE 1
	*/
	int state;
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
	tm* serve_time_begin;
	tm* serve_time_end;
		
public:
	//���캯���������
	ServiceObject();
	ServiceObject(float defaultTemp, float defaultSpeed, int defaultmode);
	//���������������
	~ServiceObject();
	//��ȡϵͳʱ�䣬����tm�ṹ
	tm* gettime();
	//ע��serveID����scheduler����
	void assignServe(int ServeID);
	//��ʼ����������Ӧ�����ͷ���ʼʱ��
	void startWork(int RoomID, float ctemp,float ttemp, int speed, int mode, float lastfee);
	//ֹͣ����,�������÷������ʱ��
	void endWork();
	//get����
	float getFee();
	int getRoomID();
	int getServerID();
	float getCurrenTemp();
	float getTargetTemp();
	int getFanSpeed();
	int getstate();
	int getMode();
	tm* getServeBeginTime();
	tm* getServeEndTime();
	//�޸ķ�������Ŀ���¶Ⱥͷ���
	void modifyTtemp(float ttemp);
	void modifyFanSpeed(float speed);
	//���µ�ǰ���ºͷ���
	void updatectemp(float speed);
	void updatefee(float fee);
	//��ȡ��ǰ�������ʱ������������
	int getServeTime();	
	
};

class queueServiceObject : public QObject
{
	Q_OBJECT

public:
	queueServiceObject(QObject *parent);
	~queueServiceObject();
	//��ʱ10s�������񣬼��㵱ǰ�¶�
	void service();
	//���ݷ���š�����Ų��ҷ������
	ServiceObject* getServerObjectRoomID(int RoomID);
	ServiceObject* getServerObjectServerID(int ServerID);
	//���������ʱ�䡢���ȼ����͵�������÷������
	ServiceObject* getServerObjectLongest();
	ServiceObject* getServerObjectLongest(int FanSpeed);
	//��Ӻ�ɾ���������
	void addserver(ServiceObject* so);
	void deleteserver(int ServiceID);
	//���·������
	/*
	todo:��ɷ��Ͱ��������ݿ�д��Ϣ
	*/
	RequestObject* updateService(int ServerID, RequestObject* ro);
	int size();
	int getaserveID();
signals:
	//signal���������ݿ�
	//void updatedb(ServiceObject*);
	//signal���������
	void servicefinish(int);
protected:
	QVector<ServiceObject*> queue;
};

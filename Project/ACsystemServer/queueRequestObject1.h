#pragma once
#include <QObject>
#include<ctime>
#include<vector>
#include<qdebug.h>
#include<qtimer.h>
#define LOW_FANSPEED 0
#define MID_FANSPEED 1
#define HIGH_FANSPEED 2
//制冷制热
#define REFRIGERATION 0
#define HEATING 1
//工作状态
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
	风速三挡分别记为
	#define LOW_FANSPEED 3
    #define MID_FANSPEED 2
    #define HIGH_FANSPEED 1
	*/
	int FanSeed;
	/*
	制冷制热
	#define REFRIGERATION 1
	#define HEATING 2
	*/
	int Mode;
	//目前为止本请求对象的总费用
	float Fee;
	//时间片
	int TimeSlice;

public:
	RequestObject();
	RequestObject(float defaultTemp, float defaultSpeed, int defaultmode);
	~RequestObject();
	void assignWait(int waitID);
	void startWait(int RoomID,float ctemp,float ttemp,int speed,float Fee,int Mode);
	//get获取信息的函数
	float getFee();
	int getRoomID();
	int getWaitID();
	float getCurrenTemp();
	float getTargetTemp();
	int getFanSpeed();
	int getTimeSlice();
	int getMode();
	//修改目标温度和风速
	void modifyTtemp(float ttemp);
	void modifyFanSpeed(float speed);
	//时间片减少
	void modifyTimeSlice(int newtimeslice);
	//更新当前室温
	void updatectemp(float speed);
};

class queueRequestObject : public QObject
{
	Q_OBJECT

public:
	queueRequestObject(QObject *parent);
	~queueRequestObject();
	void addRequestObject(RequestObject* ro);
	//每10s刷新一次当前队列中请求对象的时间片长度
	void refreshTimeSlice();
	//根据房间号，等待ID，最短时间片(更高的优先级)找到对应请求对象的地址
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

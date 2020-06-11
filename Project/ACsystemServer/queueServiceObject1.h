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
//制冷制热
#define REFRIGERATION 0
#define HEATING 1
//工作状态
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
	工作状态
    #define WORKING 0
    #define IDLE 1
	*/
	int state;
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
	//目前为止本服务对象的总费用
	float Fee;
	tm* serve_time_begin;
	tm* serve_time_end;
		
public:
	//构造函数，待添加
	ServiceObject();
	ServiceObject(float defaultTemp, float defaultSpeed, int defaultmode);
	//析构函数，待添加
	~ServiceObject();
	//获取系统时间，返回tm结构
	tm* gettime();
	//注册serveID，由scheduler操作
	void assignServe(int ServeID);
	//开始服务，设置相应参数和服务开始时间
	void startWork(int RoomID, float ctemp,float ttemp, int speed, int mode, float lastfee);
	//停止服务,并且设置服务结束时间
	void endWork();
	//get操作
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
	//修改服务对象的目标温度和风速
	void modifyTtemp(float ttemp);
	void modifyFanSpeed(float speed);
	//更新当前室温和费用
	void updatectemp(float speed);
	void updatefee(float fee);
	//获取当前对象服务时长，返回秒数
	int getServeTime();	
	
};

class queueServiceObject : public QObject
{
	Q_OBJECT

public:
	queueServiceObject(QObject *parent);
	~queueServiceObject();
	//定时10s启动服务，计算当前温度
	/*
	todo：完成服务向数据库中写房间信息（使用信号槽）。
	*/
	void service();
	//根据房间号、服务号查找服务对象
	ServiceObject* getServerObjectRoomID(int RoomID);
	ServiceObject* getServerObjectServerID(int ServerID);
	//根据最长服务时间、优先级更低的条件获得服务对象
	ServiceObject* getServerObjectLongest();
	ServiceObject* getServerObjectLongest(int FanSpeed);
	//添加和删除服务对象
	void addserver(ServiceObject* so);
	void deleteserver(int ServiceID);
	//更新服务对象
	/*
	todo:完成发送包和向数据库写信息
	*/
	RequestObject* updateService(int ServerID, RequestObject* ro);
	int size();
	int getaserveID();
signals:
	//signal：更新数据库
	//void updatedb(ServiceObject*);
	//signal：服务完成
	void servicefinish(int);
protected:
	QVector<ServiceObject*> queue;
};

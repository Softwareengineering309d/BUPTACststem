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
	//设置开机参数
	void setParm(int mode, float temp, int speed, float htemp, float ltemp,float feerate);
	void ready();
	//获得各房间当前状态
	QVector<int> getRoomState();  //@注：返回值应为房间信息的QVector，先写成了void，记得要改

	//创建等待队列，创建调度对象时调用
	void createWaitingQueue();
	//创建服务队列，创建调度对象时调用
	void createServiceQueue();

	//布尔值，标志调度对象是否已就绪
	bool isReady;
	/*
	todo：完成从数据库中查询上次费用的函数
	*/
	void requestOn(int RoomID);

	void requestOff(int RoomID);
	/*
	todo:根据返回值选择给用户的修改温度的请求回成功/不在调温范围的ACK
	*/
	bool changetargetTemp(int RoomID, float targetTemp);
	/*
	todo:完成调度
	*/
	void changeFanSpeed(int RoomID, int FanSpeed);
	//向数据库中写入新数据
	void changeCurrentTemp(int RoomID, float CurrentTemp);
	/*
	槽函数:endwork
	待完成： 2.房间信息写入数据库
	*/
	void endwork(int ServeID);
	/*
	槽函数：endwait
	待完成： 2. 房间信息写入数据库
	*/
	void endwait(int WaitID);
signals:
	void turnonOK(int, float, int, bool);
	void turnonEROOR(int, bool);
	void turnOffOK(int,bool);
	void changeTempBack(int, bool);
	void changeFanSpeedBack(int, bool);
	void serviceStart(int, int);
	void serviceFinish(int);
	void waitStart(int, int, int);
	void keepAlive(int, int , float);
	/*
	todo: 数据库操作的信号
	*/
};

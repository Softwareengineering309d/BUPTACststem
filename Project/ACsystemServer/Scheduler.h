#pragma once

#include <QObject>

class Scheduler : public QObject
{
	Q_OBJECT

public:
	Scheduler(QObject *parent);
	~Scheduler();
	//设置参数，把调度对象的几个属性设置成与传参一致
	int setPara(int defaultMode, float tempHighLimit, float tempLowLimit, float defaultTargetTemp, float defaultFeeRate);
	//将调度对象的状态设置为Ready
	int ready();
	//获得各房间当前状态
	void getRoomState();  //注：返回值应为房间信息的数组，先写成了void，记得要改

private:
	//创建等待队列，创建调度对象时调用
	int createWaitingQueue();
	//创建服务队列，创建调度对象时调用
	int createServiceQueue();

	//创建服务对象时使用，默认模式
	int defaultMode;
	//创建服务对象时使用，最高温度
	float tempHighLimit;
	//创建服务对象时使用，最低温度
	float tempLowLimit;
	//创建服务对象时使用，默认目标温度
	float defaultTargetTemp;
	//创建服务对象时使用，默认费率
	float defaultFeeRate;
	//布尔值，标志调度对象是否已就绪
	bool isReady;
};

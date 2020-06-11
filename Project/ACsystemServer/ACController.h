#pragma once

#include <QObject>
#include"Scheduler.h"


class ACController : public QObject
{
	Q_OBJECT

public:
	ACController(QObject *parent);
	~ACController();
	//槽函数，点击开机按钮或者打开程序时触发
	void powerOn(Scheduler* scheduler);
	//槽函数，管理员开机输入参数时调用，传送参数到调度对象
	void sendPara(Scheduler* scheduler,int defaultMode, float tempHighLimit, float tempLowLimit, float defaultTargetTemp, int FanSpeed,float defaultFeeRate);
	//槽函数，管理员点击开始运行时调用
	void startUp(Scheduler* scheduler);
	//每隔一定时间调用一次，获取房间当前状态并展示在界面上
	QVector<int> checkRoomState(Scheduler* scheduler);  //@注：应改为QVector<RoomBean>或者Room，总之就是一个存储有房间信息的东西

private:
	//由powerOn()函数调用，创建调度对象
	int createScheduler(Scheduler* scheduler);
};

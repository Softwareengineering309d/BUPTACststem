#include "Scheduler.h"

Scheduler::Scheduler(QObject *parent)
	: QObject(parent),WaitQueue(this),ServiceQueue(this)
{
	connect(&ServiceQueue, &queueServiceObject::servicefinish, this, &Scheduler::endwork);
	connect(&WaitQueue, &queueRequestObject::waitFinish, this, &Scheduler::endwait);
	timer = new QTimer(this);
	connect(timer, &QTimer::timeout, &ServiceQueue,&queueServiceObject::service);
	connect(timer, &QTimer::timeout,&WaitQueue,&queueRequestObject::refreshTimeSlice);
	timer->start(INTERVAL);
}

Scheduler::~Scheduler()
{
}

void Scheduler::setParm(int mode, float temp, int speed, float htemp, float ltemp,float feerate)
{
	this->defaultMode = mode;
	this->defaultTemp = temp;
	this->defaultFanSpeed = speed;
	this->hightlimitTemp = htemp;
	this->lowlimitTemp = ltemp;
	this->FeeRate = feerate;
}

void Scheduler::ready()
{
	this->isReady = true;
}

QVector<int> Scheduler::getRoomState()
{
	// TODO: 去DBFacade查roomState，把全部房间的状态放入roomStates。
	return QVector<int>();
}

void Scheduler::createWaitingQueue()
{
}

void Scheduler::createServiceQueue()
{
}

void Scheduler::requestOn(int RoomID)
{
	float lastfee=0;
	/*
	todo 从数据库中获取上次服务为止的总费用
	lastfee=...
	*/
	if (ServiceQueue.size() < 3)
	{
		int serveID = ServiceQueue.getaserveID();
		ServiceObject* so = new ServiceObject();
		so->assignServe(serveID);
		so->startWork(RoomID, 0, defaultTemp, defaultFanSpeed, defaultMode, lastfee);
		ServiceQueue.addserver(so);
		emit turnonOK(RoomID, defaultTemp, defaultFanSpeed,defaultMode, true);
		emit serviceStart(RoomID, serveID);
	}
	else
	{
		int waitID = WaitQueue.getawaitid();
		RequestObject* ro = new RequestObject();
		ro->assignWait(waitID);
		ro->startWait(RoomID, 0, defaultTemp, defaultFanSpeed, defaultMode, lastfee);
		WaitQueue.addRequestObject(ro);
		emit turnonOK(RoomID, defaultTemp, defaultFanSpeed, defaultMode,true);
	}
	
}

void Scheduler::requestOff(int RoomID)
{
	//关机房间在服务队列中
	if (ServiceQueue.getServerObjectRoomID(RoomID) != nullptr)
	{
		RequestObject* ro;
		if (!WaitQueue.empty())
		{
			ro = WaitQueue.getRequestObjectShortest(ServiceQueue.getServerObjectRoomID(RoomID)->getFanSpeed());
			int serviceid = ServiceQueue.getServerObjectRoomID(RoomID)->getServerID();
			ServiceQueue.updateService(serviceid, ro);
			WaitQueue.deleteRequest(ro->getWaitID());
			emit serviceStart(ro->getRoomID(), serviceid);
		}
		ServiceQueue.deleteserver(ServiceQueue.getServerObjectRoomID(RoomID)->getServerID());
	}
	//关机房间在等待队列中
	else {
		int waitID = WaitQueue.getRequestObjectRoomID(RoomID)->getWaitID();
		WaitQueue.deleteRequest(waitID);
	}
	emit turnOffOK(RoomID, true);
}

bool Scheduler::changetargetTemp(int RoomID, float targetTemp)
{
	if (targetTemp > hightlimitTemp|| targetTemp < lowlimitTemp)
	{
		emit changeFanSpeedBack(RoomID, false);
		return false;
	}
	else 
	{
		if (ServiceQueue.getServerObjectRoomID(RoomID) != nullptr)
		{
			ServiceQueue.getServerObjectRoomID(RoomID)->modifyTtemp(targetTemp);
		}
		else {
			WaitQueue.getRequestObjectRoomID(RoomID)->modifyTtemp(targetTemp);
		}
		return true;
		emit changeTempBack(RoomID, true);
	}

}

void Scheduler::changeFanSpeed(int RoomID, int FanSpeed)
{
	if (ServiceQueue.getServerObjectRoomID(RoomID) != nullptr)
	{
		ServiceQueue.getServerObjectRoomID(RoomID)->modifyFanSpeed(FanSpeed);
		/*
		todo:完成修改服务队列中的风速的调度
		*/
		RequestObject* ro = WaitQueue.getRequestObjectShortest(FanSpeed);
		if (ro!= nullptr)
		{
			ServiceQueue.getServerObjectRoomID(RoomID)->endWork();
			int serveid = ServiceQueue.getServerObjectRoomID(RoomID)->getServerID();
			int waitid = ro->getWaitID();
			RequestObject* ro2=ServiceQueue.updateService(serveid, ro);
			WaitQueue.deleteRequest(waitid);
			WaitQueue.addRequestObject(ro2);
			emit serviceStart(ro->getRoomID(), serveid);
			emit waitStart(ro2->getRoomID(), ro2->getWaitID(), ro2->getTimeSlice());
		}
	}
	else {
		WaitQueue.getRequestObjectRoomID(RoomID)->modifyFanSpeed(FanSpeed);
		/*
		todo:完成修改等待队列中的风速的调度
		*/
		RequestObject *ro = WaitQueue.getRequestObjectRoomID(RoomID);
		if (FanSpeed == HIGH_FANSPEED)
		{
			ServiceObject* so = ServiceQueue.getServerObjectLongest(LOW_FANSPEED);
			//服务队列中存在低风速，优先替换低风速服务时长最长的
			if (so != nullptr)
			{
				so->endWork();
				int sID = so->getServerID();
				int wID = ro->getWaitID();
				RequestObject* ro2 = ServiceQueue.updateService(sID, ro);
				WaitQueue.deleteRequest(wID);
				WaitQueue.addRequestObject(ro2);
				emit serviceStart(RoomID, sID);
				emit waitStart(ro2->getRoomID(), wID,ro2->getTimeSlice());
			}
			else {
				//服务队列中存在中风速，优先替换中风速服务时长最长的
				so = ServiceQueue.getServerObjectLongest(MID_FANSPEED);
				if (so != nullptr)
				{
					so->endWork();
					int sID = so->getServerID();
					int wID = ro->getWaitID();
					RequestObject* ro2 = ServiceQueue.updateService(sID, ro);
					WaitQueue.deleteRequest(wID);
					WaitQueue.addRequestObject(ro2);
					emit serviceStart(RoomID, sID);
					emit waitStart(ro2->getRoomID(), wID, ro2->getTimeSlice());
				}
				//服务队列中不存在低风速高风速，优先替换高风速服务时长最长的
				else {
					so = ServiceQueue.getServerObjectLongest(HIGH_FANSPEED);
					so->endWork();
					int sID = so->getServerID();
					int wID = ro->getWaitID();
					RequestObject* ro2 = ServiceQueue.updateService(sID, ro);
					WaitQueue.deleteRequest(wID);
					WaitQueue.addRequestObject(ro2);
					emit serviceStart(RoomID, sID);
					emit waitStart(ro2->getRoomID(), wID, ro2->getTimeSlice());
				}
			}
		}
		else if (FanSpeed == MID_FANSPEED)
		{
			ServiceObject* so = ServiceQueue.getServerObjectLongest(LOW_FANSPEED);
			//服务队列中存在低风速，优先替换低风速服务时长最长的
			if (so != nullptr)
			{
				so->endWork();
				int sID = so->getServerID();
				int wID = ro->getWaitID();
				RequestObject* ro2 = ServiceQueue.updateService(sID, ro);
				WaitQueue.deleteRequest(wID);
				WaitQueue.addRequestObject(ro2);
				emit serviceStart(RoomID, sID);
				emit waitStart(ro2->getRoomID(), wID, ro2->getTimeSlice());
			}
			else {
				//服务队列中没有低风速但是存在中风速，优先替换中风速服务时长最长的
				so = ServiceQueue.getServerObjectLongest(MID_FANSPEED);
				if (so != nullptr)
				{
					so->endWork();
					int sID = so->getServerID();
					int wID = ro->getWaitID();
					RequestObject* ro2 = ServiceQueue.updateService(sID, ro);
					WaitQueue.deleteRequest(wID);
					WaitQueue.addRequestObject(ro2);
					emit serviceStart(RoomID, sID);
					emit waitStart(ro2->getRoomID(), wID, ro2->getTimeSlice());
				}
			}

		}
		//切换成低风速的情况
		else {
			ServiceObject* so = ServiceQueue.getServerObjectLongest(LOW_FANSPEED);
			if (so != nullptr)
			{
				so->endWork();
				int sID = so->getServerID();
				int wID = ro->getWaitID();
				RequestObject* ro2 = ServiceQueue.updateService(sID, ro);
				WaitQueue.deleteRequest(wID);
				WaitQueue.addRequestObject(ro2);
				emit serviceStart(RoomID, sID);
				emit waitStart(ro2->getRoomID(), wID, ro2->getTimeSlice());
			}
		}
	}
	emit changeFanSpeedBack(RoomID, true);
}

void Scheduler::changeCurrentTemp(int RoomID, float CurrentTemp)
{
	if (ServiceQueue.getServerObjectRoomID(RoomID) != nullptr)
	{
		ServiceQueue.getServerObjectRoomID(RoomID)->updatectemp(CurrentTemp);
	}
	else {
		WaitQueue.getRequestObjectRoomID(RoomID)->updatectemp(CurrentTemp);
	}

}

void Scheduler::endwork(int ServeID)
{
	if (!WaitQueue.empty())
	{
		RequestObject* ro = WaitQueue.getRequestObjectShortest();
		emit serviceFinish(ServiceQueue.getServerObjectServerID(ServeID)->getRoomID());
		ServiceQueue.updateService(ServeID, ro);
		WaitQueue.deleteRequest(ro->getWaitID());
		emit serviceStart(ro->getRoomID(),ServeID);
	}
}

void Scheduler::endwait(int WaitID)
{
	int speed = WaitQueue.getRequestObjectWaitID(WaitID)->getFanSpeed();
	ServiceObject* so = ServiceQueue.getServerObjectLongest(speed);
	RequestObject* ro = WaitQueue.getRequestObjectWaitID(WaitID);
	if (so != nullptr)
	{
		so->endWork();
		int sid=so->getServerID();
		RequestObject* ro2 = new RequestObject();
		ro2 = ServiceQueue.updateService(so->getServerID(), ro);
		WaitQueue.deleteRequest(WaitID);
		WaitQueue.addRequestObject(ro2);
		emit serviceStart(ro->getRoomID(), sid);
		emit waitStart(ro2->getRoomID(), WaitID,ro2->getTimeSlice());
	}
	else {
		/*
		没找到优先级比时间片走完的等待对象低的服务对象，重新设置时间片
		*/
		WaitQueue.getRequestObjectWaitID(WaitID)->modifyTimeSlice(120);
	}



}

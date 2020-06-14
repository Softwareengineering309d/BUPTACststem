#include "Scheduler.h"
#include "global.h"
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
	lastfee=systemserver->dbfacade.queryFee(RoomID);
	systemserver->dbfacade.insertOpt(RoomID, 1, OPEN, systemserver->dbfacade.gettime());
	if (ServiceQueue.size() < 3)
	{
		int serveID = ServiceQueue.getaserveID();
		ServiceObject* so = new ServiceObject();
		so->assignServe(serveID);
		so->startWork(RoomID, 0, defaultTemp, defaultFanSpeed, defaultMode, lastfee);
		ServiceQueue.addserver(so);
		emit turnonOK(RoomID, defaultTemp, defaultFanSpeed,defaultMode, true);
		emit serviceStart(RoomID, serveID);
		systemserver->dbfacade.insertRoom(RoomID, defaultMode, SERVICE, 0, defaultTemp, defaultFanSpeed, lastfee, FEERATE);
		RoomUp->update_server(RoomID, 1);
	}
	else
	{
		int waitID = WaitQueue.getawaitid();
		RequestObject* ro = new RequestObject();
		ro->assignWait(waitID);
		ro->startWait(RoomID, 0, defaultTemp, defaultFanSpeed, defaultMode, lastfee);
		WaitQueue.addRequestObject(ro);
		emit turnonOK(RoomID, defaultTemp, defaultFanSpeed, defaultMode,true);
		systemserver->dbfacade.insertRoom(RoomID, defaultMode, WAIT, 0, defaultTemp, defaultFanSpeed, lastfee, FEERATE);
		RoomUp->update_server(RoomID, 2);
	}
	
}

void Scheduler::requestOff(int RoomID)
{
	//关机房间在服务队列中
	systemserver->dbfacade.insertOpt(RoomID, 1, RCLOSE, systemserver->dbfacade.gettime());
	if (ServiceQueue.getServerObjectRoomID(RoomID) != nullptr)
	{
		RequestObject* ro;
		if (!WaitQueue.empty())
		{
			ro = WaitQueue.getRequestObjectShortest(ServiceQueue.getServerObjectRoomID(RoomID)->getFanSpeed());
			ServiceObject* so = ServiceQueue.getServerObjectRoomID(RoomID);
			so->endWork();
			systemserver->dbfacade.insertService(RoomID, so->getServeBeginTime(), so->getServeEndTime(), so->getServeTime(), so->getFee(), so->getFanSpeed());
			int serviceid = ServiceQueue.getServerObjectRoomID(RoomID)->getServerID();
			ServiceQueue.updateService(serviceid, ro);
			WaitQueue.deleteRequest(ro->getWaitID());
			emit serviceStart(ro->getRoomID(), serviceid);
			RoomUp->update_server(RoomID, 0);
			systemserver->dbfacade.updateStat(RoomID, CLOSE);
		}
		ServiceQueue.deleteserver(ServiceQueue.getServerObjectRoomID(RoomID)->getServerID());
	}
	//关机房间在等待队列中
	else {
		int waitID = WaitQueue.getRequestObjectRoomID(RoomID)->getWaitID();
		WaitQueue.deleteRequest(waitID);
		RoomUp->update_server(RoomID, 0);
		systemserver->dbfacade.updateStat(RoomID, CLOSE);
	}
	emit turnOffOK(RoomID, true);
}

bool Scheduler::changetargetTemp(int RoomID, float targetTemp)
{
	systemserver->dbfacade.insertOpt(RoomID, 1, CHANGETEMP, systemserver->dbfacade.gettime());
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
			ServiceObject* so = ServiceQueue.getServerObjectRoomID(RoomID);
			RoomUp->update_timing(RoomID, 1, so->getCurrenTemp(), so->getTargetTemp(), so->getFanSpeed(), so->getFee());
			systemserver->dbfacade.updatetartemp(RoomID, targetTemp);
		}
		else {
			WaitQueue.getRequestObjectRoomID(RoomID)->modifyTtemp(targetTemp);
			RequestObject* ro = WaitQueue.getRequestObjectRoomID(RoomID);
			RoomUp->update_timing(RoomID, 2, ro->getCurrenTemp(), ro->getTargetTemp(), ro->getFanSpeed(), ro->getFee());
			systemserver->dbfacade.updatetartemp(RoomID, targetTemp);
		}
		return true;
		emit changeTempBack(RoomID, true);
	}

}

void Scheduler::changeFanSpeed(int RoomID, int FanSpeed)
{
	systemserver->dbfacade.insertOpt(RoomID, 1, CHANGESPEED, systemserver->dbfacade.gettime());
	if (ServiceQueue.getServerObjectRoomID(RoomID) != nullptr)
	{
		ServiceQueue.getServerObjectRoomID(RoomID)->modifyFanSpeed(FanSpeed);
		/*
		todo:完成修改服务队列中的风速的调度
		*/
		RequestObject* ro = WaitQueue.getRequestObjectShortest(FanSpeed);
		systemserver->dbfacade.updatespeed(RoomID,FanSpeed);
		if (ro!= nullptr)
		{
			ServiceQueue.getServerObjectRoomID(RoomID)->endWork();
			ServiceObject* so = ServiceQueue.getServerObjectRoomID(RoomID);
			systemserver->dbfacade.insertService(RoomID, so->getServeBeginTime(), so->getServeEndTime(), so->getServeTime(), so->getFee(), so->getFanSpeed());
			int serveid = ServiceQueue.getServerObjectRoomID(RoomID)->getServerID();
			int waitid = ro->getWaitID();
			RequestObject* ro2=ServiceQueue.updateService(serveid, ro);
			WaitQueue.deleteRequest(waitid);
			WaitQueue.addRequestObject(ro2);
			emit serviceStart(ro->getRoomID(), serveid);
			systemserver->dbfacade.updateStat(RoomID, SERVICE);
			RoomUp->update_server(ro->getRoomID(), 1);
			emit waitStart(ro2->getRoomID(), ro2->getWaitID(), ro2->getTimeSlice());
			systemserver->dbfacade.updateStat(RoomID, WAIT);
			RoomUp->update_server(ro2->getRoomID(), 2);
		}
	}
	else {
		WaitQueue.getRequestObjectRoomID(RoomID)->modifyFanSpeed(FanSpeed);
		systemserver->dbfacade.updatespeed(RoomID, FanSpeed);
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
				systemserver->dbfacade.insertService(RoomID, so->getServeBeginTime(), so->getServeEndTime(), so->getServeTime(), so->getFee(), so->getFanSpeed());
				int sID = so->getServerID();
				int wID = ro->getWaitID();
				RequestObject* ro2 = ServiceQueue.updateService(sID, ro);
				WaitQueue.deleteRequest(wID);
				WaitQueue.addRequestObject(ro2);
				emit serviceStart(RoomID, sID);
				systemserver->dbfacade.updateStat(RoomID, SERVICE);
				RoomUp->update_server(RoomID, 1);
				emit waitStart(ro2->getRoomID(), wID,ro2->getTimeSlice());
				systemserver->dbfacade.updateStat(RoomID, WAIT);
				RoomUp->update_server(ro2->getRoomID(), 2);
			}
			else {
				//服务队列中存在中风速，优先替换中风速服务时长最长的
				so = ServiceQueue.getServerObjectLongest(MID_FANSPEED);
				if (so != nullptr)
				{
					so->endWork();
					systemserver->dbfacade.insertService(RoomID, so->getServeBeginTime(), so->getServeEndTime(), so->getServeTime(), so->getFee(), so->getFanSpeed());
					int sID = so->getServerID();
					int wID = ro->getWaitID();
					RequestObject* ro2 = ServiceQueue.updateService(sID, ro);
					WaitQueue.deleteRequest(wID);
					WaitQueue.addRequestObject(ro2);
					emit serviceStart(RoomID, sID);
					systemserver->dbfacade.updateStat(RoomID, SERVICE);
					RoomUp->update_server(RoomID, 1);
					emit waitStart(ro2->getRoomID(), wID, ro2->getTimeSlice());
					systemserver->dbfacade.updateStat(RoomID, WAIT);
					RoomUp->update_server(ro2->getRoomID(), 2);
				}
				//服务队列中不存在低风速高风速，优先替换高风速服务时长最长的
				else {
					so = ServiceQueue.getServerObjectLongest(HIGH_FANSPEED);
					so->endWork();
					systemserver->dbfacade.insertService(RoomID, so->getServeBeginTime(), so->getServeEndTime(), so->getServeTime(), so->getFee(), so->getFanSpeed());
					int sID = so->getServerID();
					int wID = ro->getWaitID();
					RequestObject* ro2 = ServiceQueue.updateService(sID, ro);
					WaitQueue.deleteRequest(wID);
					WaitQueue.addRequestObject(ro2);
					emit serviceStart(RoomID, sID);
					systemserver->dbfacade.updateStat(RoomID, SERVICE);
					RoomUp->update_server(RoomID, 1);
					emit waitStart(ro2->getRoomID(), wID, ro2->getTimeSlice());
					systemserver->dbfacade.updateStat(RoomID, SERVICE);
					RoomUp->update_server(ro2->getRoomID(), 2);
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
				systemserver->dbfacade.insertService(RoomID, so->getServeBeginTime(), so->getServeEndTime(), so->getServeTime(), so->getFee(), so->getFanSpeed());
				int sID = so->getServerID();
				int wID = ro->getWaitID();
				RequestObject* ro2 = ServiceQueue.updateService(sID, ro);
				WaitQueue.deleteRequest(wID);
				WaitQueue.addRequestObject(ro2);
				emit serviceStart(RoomID, sID);
				systemserver->dbfacade.updateStat(RoomID, SERVICE);
				RoomUp->update_server(RoomID, 1);
				emit waitStart(ro2->getRoomID(), wID, ro2->getTimeSlice());
				systemserver->dbfacade.updateStat(RoomID, WAIT);
				RoomUp->update_server(ro2->getRoomID(), 2);
			}
			else {
				//服务队列中没有低风速但是存在中风速，优先替换中风速服务时长最长的
				so = ServiceQueue.getServerObjectLongest(MID_FANSPEED);
				if (so != nullptr)
				{
					so->endWork();
					systemserver->dbfacade.insertService(RoomID, so->getServeBeginTime(), so->getServeEndTime(), so->getServeTime(), so->getFee(), so->getFanSpeed());
					int sID = so->getServerID();
					int wID = ro->getWaitID();
					RequestObject* ro2 = ServiceQueue.updateService(sID, ro);
					WaitQueue.deleteRequest(wID);
					WaitQueue.addRequestObject(ro2);
					emit serviceStart(RoomID, sID);
					systemserver->dbfacade.updateStat(RoomID, SERVICE);
					RoomUp->update_server(RoomID, 1);
					emit waitStart(ro2->getRoomID(), wID, ro2->getTimeSlice());
					systemserver->dbfacade.updateStat(RoomID, WAIT);
					RoomUp->update_server(ro2->getRoomID(), 2);
				}
			}

		}
		//切换成低风速的情况
		else {
			ServiceObject* so = ServiceQueue.getServerObjectLongest(LOW_FANSPEED);
			if (so != nullptr)
			{
				so->endWork();
				systemserver->dbfacade.insertService(RoomID, so->getServeBeginTime(), so->getServeEndTime(), so->getServeTime(), so->getFee(), so->getFanSpeed());
				int sID = so->getServerID();
				int wID = ro->getWaitID();
				RequestObject* ro2 = ServiceQueue.updateService(sID, ro);
				WaitQueue.deleteRequest(wID);
				WaitQueue.addRequestObject(ro2);
				emit serviceStart(RoomID, sID);
				systemserver->dbfacade.updateStat(RoomID, SERVICE);
				RoomUp->update_server(RoomID, 1);
				emit waitStart(ro2->getRoomID(), wID, ro2->getTimeSlice());
				systemserver->dbfacade.updateStat(RoomID, WAIT);
				RoomUp->update_server(ro2->getRoomID(), 2);
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
		ServiceObject* so = ServiceQueue.getServerObjectRoomID(RoomID);
		RoomUp->update_timing(RoomID, 1, so->getCurrenTemp(), so->getTargetTemp(), so->getFanSpeed(), so->getFee());
		systemserver->dbfacade.updatecurtemp(RoomID, CurrentTemp);
	}
	else {
		WaitQueue.getRequestObjectRoomID(RoomID)->updatectemp(CurrentTemp);
		RequestObject* ro = WaitQueue.getRequestObjectRoomID(RoomID);
		RoomUp->update_timing(RoomID, 1, ro->getCurrenTemp(), ro->getTargetTemp(), ro->getFanSpeed(), ro->getFee());
		systemserver->dbfacade.updatecurtemp(RoomID, CurrentTemp);
	}

}

void Scheduler::endwork(int ServeID)
{
	
	if (!WaitQueue.empty())
	{
		RequestObject* ro = WaitQueue.getRequestObjectShortest();
		emit serviceFinish(ServiceQueue.getServerObjectServerID(ServeID)->getRoomID());
		systemserver->dbfacade.updateStat(ServiceQueue.getServerObjectServerID(ServeID)->getRoomID(), CLOSE);
		ServiceObject* so = ServiceQueue.getServerObjectServerID(ServeID);
		so->endWork();
		systemserver->dbfacade.insertService(so->getRoomID(), so->getServeBeginTime(), so->getServeEndTime(), so->getServeTime(), so->getFee(), so->getFanSpeed());
		RoomUp->update_server(ServiceQueue.getServerObjectServerID(ServeID)->getRoomID(), 0);
		RoomUp->update_server(ro->getRoomID(), 1);
		ServiceQueue.updateService(ServeID, ro);
		WaitQueue.deleteRequest(ro->getWaitID());
		emit serviceStart(ro->getRoomID(),ServeID);
		systemserver->dbfacade.updateStat(ro->getRoomID(),SERVICE);
		
	}
	else {
		emit serviceFinish(ServiceQueue.getServerObjectServerID(ServeID)->getRoomID());
		systemserver->dbfacade.updateStat(ServiceQueue.getServerObjectServerID(ServeID)->getRoomID(), CLOSE);
		RoomUp->update_server(ServiceQueue.getServerObjectServerID(ServeID)->getRoomID(), 0);
		ServiceObject* so = ServiceQueue.getServerObjectServerID(ServeID);
		so->endWork();
		systemserver->dbfacade.insertService(so->getRoomID(), so->getServeBeginTime(), so->getServeEndTime(), so->getServeTime(), so->getFee(), so->getFanSpeed());
		ServiceQueue.deleteserver(ServeID);
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
		systemserver->dbfacade.insertService(so->getRoomID(), so->getServeBeginTime(), so->getServeEndTime(), so->getServeTime(), so->getFee(), so->getFanSpeed());
		systemserver->dbfacade.updateStat(so->getRoomID(), WAIT);
		int sid=so->getServerID();
		RequestObject* ro2 = new RequestObject();
		ro2 = ServiceQueue.updateService(so->getServerID(), ro);
		WaitQueue.deleteRequest(WaitID);
		WaitQueue.addRequestObject(ro2);
		emit serviceStart(ro->getRoomID(), sid);
		systemserver->dbfacade.updateStat(ro->getRoomID(), SERVICE);
		RoomUp->update_server(ro->getRoomID(), 1);
		emit waitStart(ro2->getRoomID(), WaitID,ro2->getTimeSlice());
		RoomUp->update_server(ro2->getRoomID(), 2);
	}
	else {
		/*
		没找到优先级比时间片走完的等待对象低的服务对象，重新设置时间片
		*/
		WaitQueue.getRequestObjectWaitID(WaitID)->modifyTimeSlice(120);
	}



}

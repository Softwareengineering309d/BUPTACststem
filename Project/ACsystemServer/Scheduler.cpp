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
	// TODO: ȥDBFacade��roomState����ȫ�������״̬����roomStates��
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
	//�ػ������ڷ��������
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
			RoomUp->update_server(RoomID, 0);
		}
		ServiceQueue.deleteserver(ServiceQueue.getServerObjectRoomID(RoomID)->getServerID());
	}
	//�ػ������ڵȴ�������
	else {
		int waitID = WaitQueue.getRequestObjectRoomID(RoomID)->getWaitID();
		WaitQueue.deleteRequest(waitID);
		RoomUp->update_server(RoomID, 0);
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
			ServiceObject* so = ServiceQueue.getServerObjectRoomID(RoomID);
			RoomUp->update_timing(RoomID, 1, so->getCurrenTemp(), so->getTargetTemp(), so->getFanSpeed(), so->getFee());
		}
		else {
			WaitQueue.getRequestObjectRoomID(RoomID)->modifyTtemp(targetTemp);
			RequestObject* ro = WaitQueue.getRequestObjectRoomID(RoomID);
			RoomUp->update_timing(RoomID, 2, ro->getCurrenTemp(), ro->getTargetTemp(), ro->getFanSpeed(), ro->getFee());
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
		todo:����޸ķ�������еķ��ٵĵ���
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
			RoomUp->update_server(ro->getRoomID(), 1);
			emit waitStart(ro2->getRoomID(), ro2->getWaitID(), ro2->getTimeSlice());
			RoomUp->update_server(ro2->getRoomID(), 2);
		}
	}
	else {
		WaitQueue.getRequestObjectRoomID(RoomID)->modifyFanSpeed(FanSpeed);
		/*
		todo:����޸ĵȴ������еķ��ٵĵ���
		*/
		RequestObject *ro = WaitQueue.getRequestObjectRoomID(RoomID);
		if (FanSpeed == HIGH_FANSPEED)
		{
			ServiceObject* so = ServiceQueue.getServerObjectLongest(LOW_FANSPEED);
			//��������д��ڵͷ��٣������滻�ͷ��ٷ���ʱ�����
			if (so != nullptr)
			{
				so->endWork();
				int sID = so->getServerID();
				int wID = ro->getWaitID();
				RequestObject* ro2 = ServiceQueue.updateService(sID, ro);
				WaitQueue.deleteRequest(wID);
				WaitQueue.addRequestObject(ro2);
				emit serviceStart(RoomID, sID);
				RoomUp->update_server(RoomID, 1);
				emit waitStart(ro2->getRoomID(), wID,ro2->getTimeSlice());
				RoomUp->update_server(ro2->getRoomID(), 2);
			}
			else {
				//��������д����з��٣������滻�з��ٷ���ʱ�����
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
					RoomUp->update_server(RoomID, 1);
					emit waitStart(ro2->getRoomID(), wID, ro2->getTimeSlice());
					RoomUp->update_server(ro2->getRoomID(), 2);
				}
				//��������в����ڵͷ��ٸ߷��٣������滻�߷��ٷ���ʱ�����
				else {
					so = ServiceQueue.getServerObjectLongest(HIGH_FANSPEED);
					so->endWork();
					int sID = so->getServerID();
					int wID = ro->getWaitID();
					RequestObject* ro2 = ServiceQueue.updateService(sID, ro);
					WaitQueue.deleteRequest(wID);
					WaitQueue.addRequestObject(ro2);
					emit serviceStart(RoomID, sID);
					RoomUp->update_server(RoomID, 1);
					emit waitStart(ro2->getRoomID(), wID, ro2->getTimeSlice());
					RoomUp->update_server(ro2->getRoomID(), 2);
				}
			}
		}
		else if (FanSpeed == MID_FANSPEED)
		{
			ServiceObject* so = ServiceQueue.getServerObjectLongest(LOW_FANSPEED);
			//��������д��ڵͷ��٣������滻�ͷ��ٷ���ʱ�����
			if (so != nullptr)
			{
				so->endWork();
				int sID = so->getServerID();
				int wID = ro->getWaitID();
				RequestObject* ro2 = ServiceQueue.updateService(sID, ro);
				WaitQueue.deleteRequest(wID);
				WaitQueue.addRequestObject(ro2);
				emit serviceStart(RoomID, sID);
				RoomUp->update_server(RoomID, 1);
				emit waitStart(ro2->getRoomID(), wID, ro2->getTimeSlice());
				RoomUp->update_server(ro2->getRoomID(), 2);
			}
			else {
				//���������û�еͷ��ٵ��Ǵ����з��٣������滻�з��ٷ���ʱ�����
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
					RoomUp->update_server(RoomID, 1);
					emit waitStart(ro2->getRoomID(), wID, ro2->getTimeSlice());
					RoomUp->update_server(ro2->getRoomID(), 2);
				}
			}

		}
		//�л��ɵͷ��ٵ����
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
				RoomUp->update_server(RoomID, 1);
				emit waitStart(ro2->getRoomID(), wID, ro2->getTimeSlice());
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
	}
	else {
		WaitQueue.getRequestObjectRoomID(RoomID)->updatectemp(CurrentTemp);
		RequestObject* ro = WaitQueue.getRequestObjectRoomID(RoomID);
		RoomUp->update_timing(RoomID, 1, ro->getCurrenTemp(), ro->getTargetTemp(), ro->getFanSpeed(), ro->getFee());
	}

}

void Scheduler::endwork(int ServeID)
{
	
	if (!WaitQueue.empty())
	{
		RequestObject* ro = WaitQueue.getRequestObjectShortest();
		emit serviceFinish(ServiceQueue.getServerObjectServerID(ServeID)->getRoomID());
		RoomUp->update_server(ServiceQueue.getServerObjectServerID(ServeID)->getRoomID(), 0);
		RoomUp->update_server(ro->getRoomID(), 1);
		ServiceQueue.updateService(ServeID, ro);
		WaitQueue.deleteRequest(ro->getWaitID());
		emit serviceStart(ro->getRoomID(),ServeID);
		
	}
	else {
		emit serviceFinish(ServiceQueue.getServerObjectServerID(ServeID)->getRoomID());
		RoomUp->update_server(ServiceQueue.getServerObjectServerID(ServeID)->getRoomID(), 0);
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
		int sid=so->getServerID();
		RequestObject* ro2 = new RequestObject();
		ro2 = ServiceQueue.updateService(so->getServerID(), ro);
		WaitQueue.deleteRequest(WaitID);
		WaitQueue.addRequestObject(ro2);
		emit serviceStart(ro->getRoomID(), sid);
		RoomUp->update_server(ro->getRoomID(), 1);
		emit waitStart(ro2->getRoomID(), WaitID,ro2->getTimeSlice());
		RoomUp->update_server(ro2->getRoomID(), 2);
	}
	else {
		/*
		û�ҵ����ȼ���ʱ��Ƭ����ĵȴ�����͵ķ��������������ʱ��Ƭ
		*/
		WaitQueue.getRequestObjectWaitID(WaitID)->modifyTimeSlice(120);
	}



}

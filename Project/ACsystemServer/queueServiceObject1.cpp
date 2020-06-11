#include"queueRequestObject1.h"
#include"queueServiceObject1.h"

queueServiceObject::queueServiceObject(QObject *parent)
	: QObject(parent)
{
}

queueServiceObject::~queueServiceObject()
{
}

void queueServiceObject::service()
{
	float temp,fee,delta;
	for (int i = 0; i < size(); i++)
	{
		if (queue[i]==nullptr)
		{
			continue;
		}
		//服务完成
		else if (queue[i]->getCurrenTemp() == queue[i]->getTargetTemp())
		{
			queue[i]->endWork();
			/*
			todo1：服务完成，结果写进数据库
			connect....
			todo2: 服务完成，通过信号槽通知sheduler进行调度
			connect.....
			*/
			emit servicefinish(queue[i]->getServerID());
			continue;
		}
		//处于等待状态（可能是从服务状态切换到等待状态的一个瞬间被服务了，但是还是要避免这种情况）
		if (queue[i]->getstate() == IDLE)
		{
			continue;
		}
		switch (queue[i]->getFanSpeed())
		{
		case LOW_FANSPEED:
			delta = 1.0 / 18;
			temp = queue[i]->getCurrenTemp();
			temp = temp + delta;
			fee= delta;
			if (temp > queue[i]->getTargetTemp())
			{
				temp = queue[i]->getTargetTemp();
				fee = temp - queue[i]->getCurrenTemp();
			}
			queue[i]->updatefee(queue[i]->getFee() + fee);
			queue[i]->updatectemp(temp);
			/*
			todo1：服务完成，结果写进数据库
			connect....
			*/
			qDebug() <<"Roomid:"<< queue[i]->getRoomID()<<"  currenttemp:"<<queue[i]->getCurrenTemp()<<"  fee: "<<queue[i]->getFee()<<"  fanspeed:"<<queue[i]->getFanSpeed();
			if (temp == queue[i]->getTargetTemp())
			{
				emit servicefinish(queue[i]->getServerID());
			}
			break;
		case MID_FANSPEED:
			delta = 1.0 / 12;
			temp = queue[i]->getCurrenTemp();
			temp = temp + delta;
			fee = delta;
			if (temp > queue[i]->getTargetTemp())
			{
				temp = queue[i]->getTargetTemp();
				fee = temp - queue[i]->getCurrenTemp();
			}
			queue[i]->updatefee(queue[i]->getFee() + fee);
			queue[i]->updatectemp(temp);
			/*
			todo1：服务完成，结果写进数据库
			connect....
			*/
			qDebug() << "Roomid:" << queue[i]->getRoomID() << "  currenttemp:" << queue[i]->getCurrenTemp() << "  fee: " << queue[i]->getFee() << "  fanspeed:" << queue[i]->getFanSpeed();
			if (temp == queue[i]->getTargetTemp())
			{
				emit servicefinish(queue[i]->getServerID());
			}
			break;
		case HIGH_FANSPEED:
			delta = 1.0/ 6;
			temp = queue[i]->getCurrenTemp();
			temp = temp + delta;
			fee = delta;
			if (temp > queue[i]->getTargetTemp())
			{
				temp = queue[i]->getTargetTemp();
				fee = temp - queue[i]->getCurrenTemp();
			}
			queue[i]->updatefee(queue[i]->getFee() + fee);
			queue[i]->updatectemp(temp);
			/*
			todo1：服务完成，结果写进数据库
			connect....
			*/
			qDebug() << "Roomid:" << queue[i]->getRoomID() << "  currenttemp:" << queue[i]->getCurrenTemp() << "  fee: " << queue[i]->getFee() << "  fanspeed:" << queue[i]->getFanSpeed();
			if (temp == queue[i]->getTargetTemp())
			{
				emit servicefinish(queue[i]->getServerID());
			}
			break;
		}
	}
	
}

ServiceObject* queueServiceObject::getServerObjectRoomID(int RoomID)
{
	for (int i = 0; i <size(); i++)
	{
		if (queue[i]->getRoomID() == RoomID)
		{
			return queue[i];
		}
	}
	return nullptr;
}

ServiceObject* queueServiceObject::getServerObjectServerID(int ServerID)
{
	for (int i = 0; i < size(); i++)
	{
		if (queue[i]->getServerID() == ServerID)
		{
			return queue[i];
		}
	}
	return nullptr;
}

ServiceObject* queueServiceObject::getServerObjectLongest()
{
	int servetime=0;
	int longestServeID;
	for (int i = 0; i < size(); i++)
	{
		if (queue[i]->getServeTime() > servetime)
		{
			servetime = queue[i]->getServeTime();
			longestServeID = queue[i]->getServerID();
		}
	}
	if (longestServeID!=0)
	{
		return getServerObjectServerID(longestServeID);
	}
	return nullptr;
}

ServiceObject* queueServiceObject::getServerObjectLongest(int FanSpeed)
{
	int servetime = 0;
	int longestServeID;
	for (int i = 0; i < size(); i++)
	{
		if (queue[i]->getServeTime() > servetime&&queue[i]->getFanSpeed()<=FanSpeed)
		{
			servetime = queue[i]->getServeTime();
			longestServeID = queue[i]->getServerID();
		}
	}
	if (longestServeID != 0)
	{
		return getServerObjectServerID(longestServeID);
	}
	return nullptr;
}

void queueServiceObject::addserver(ServiceObject* so)
{
	if (queue.size() <= 3)
	{
		queue.append(so);
	}
}

void queueServiceObject::deleteserver(int ServiceID)
{
	int pos;
	for (QVector<ServiceObject*>::iterator it=queue.begin();it!=queue.end();)
	{
		if (it == nullptr)
		{
			++it;
			continue;
		}
		if ((*it)->getServerID()==ServiceID)
		{
			queue.erase(it);
		}
		else
		{
			++it;
		}
	}

}

RequestObject* queueServiceObject::updateService(int ServerID, RequestObject* ro)
{
	ServiceObject* so = getServerObjectServerID(ServerID);
	int SID = so->getServerID();
	int WID = ro->getWaitID();
	RequestObject* StoRobject = new RequestObject();
	StoRobject->assignWait(WID);
	StoRobject->startWait(so->getRoomID(), so->getCurrenTemp(), so->getTargetTemp(), so->getFanSpeed(),so->getFee(),so->getMode());
	/*
	待完成
	1.将被强占的服务对象的信息写入数据库
	2.通知被抢占对象房间被抢占。
	3.通知请求对象进行服务了。
	*/
	so->startWork(ro->getRoomID(), ro->getCurrenTemp(), ro->getTargetTemp(), ro->getFanSpeed(), ro->getMode(), ro->getFee());
	return StoRobject;
}

int queueServiceObject::size()
{
	return queue.count();
}

int queueServiceObject::getaserveID()
{
	bool s1=true, s2=true, s3=true;
	QVector<ServiceObject*>::iterator it= queue.begin();
	for (; it != queue.end(); ++it)
	{
		if ((*it)->getServerID() == 1)
		{
			s1 = false;
		}
		else if ((*it)->getServerID() == 2)
		{
			s2 = false;
		}
		else if ((*it)->getServerID() == 3)
		{
			s3 = false;
		}
	}
	if (s1)
	{
		return 1;
	}
	else if (s2)
	{
		return 2;
	}
	else if (s3)
	{
		return 3;
	}
	else
		return 0;
}

ServiceObject::ServiceObject()
{
}

ServiceObject::ServiceObject(float defaultTemp, float defaultSpeed, int defaultmode)
{
	this->TargetTemp = defaultTemp;
	this->FanSeed = defaultSpeed;
	this->Mode = defaultmode;
	this->serve_time_begin = this->gettime();
}

ServiceObject::~ServiceObject()
{
}

tm* ServiceObject::gettime()
{
	time_t now = time(0);
	tm* nowtime = localtime(&now);
	return nowtime;
}

void ServiceObject::assignServe(int ServeID)
{
	this->ServerID = ServeID;
}

void ServiceObject::startWork(int RoomID, float ctemp, float ttemp, int speed, int mode, float lastfee)
{
	this->state = WORKING;
	this->RoomID = RoomID;
	this->currentemp = ctemp;
	this->TargetTemp = ttemp;
	this->FanSeed = speed;
	this->Mode = mode;
	this->Fee = lastfee;
	this->serve_time_begin = this->gettime();
	this->serve_time_end=new tm();
}

void ServiceObject::endWork()
{
	this->state = IDLE;
	this->serve_time_end = this->gettime();
}

float ServiceObject::getFee()
{
	return this->Fee;
}

int ServiceObject::getRoomID()
{
	return this->RoomID;
}

int ServiceObject::getServerID()
{
	return this->ServerID;
}

float ServiceObject::getCurrenTemp()
{
	return this->currentemp;
}

float ServiceObject::getTargetTemp()
{
	return this->TargetTemp;
}

int ServiceObject::getFanSpeed()
{
	return this->FanSeed;
}

int ServiceObject::getstate()
{
	return this->state;
}

int ServiceObject::getMode()
{
	return this->Mode;
}

tm* ServiceObject::getServeBeginTime()
{
	return this->serve_time_begin;
}

tm* ServiceObject::getServeEndTime()
{
	return this->serve_time_end;
}

void ServiceObject::modifyTtemp(float ttemp)
{
	this->TargetTemp = ttemp;
}

void ServiceObject::modifyFanSpeed(float speed)
{
	this->FanSeed = speed;
}

void ServiceObject::updatectemp(float speed)
{
	this->currentemp = speed;
}

void ServiceObject::updatefee(float fee)
{
	this->Fee = fee;
}

int ServiceObject::getServeTime()
{
	time_t begintime = mktime(this->serve_time_begin);
	time_t endtime;
	if (mktime(this->serve_time_end)!=-1)
	{
		endtime = mktime(this->serve_time_end);
	}
	else
	{
		endtime = mktime(gettime());
	}
	return endtime-begintime;
}


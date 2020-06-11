#include "queueRequestObject1.h"


queueRequestObject::queueRequestObject(QObject *parent)
	: QObject(parent)
{


}

queueRequestObject::~queueRequestObject()
{
}

void queueRequestObject::addRequestObject(RequestObject* ro)
{
	queue.append(ro);
}

void queueRequestObject::refreshTimeSlice()
{
	QVector<RequestObject*>::iterator it;
	int interval = INTERVAL / 1000;
	int lasttime = 0;
	for (it = queue.begin(); it != queue.end();)
	{
		if (it == nullptr)
		{
			++it;
			continue;
		}
		lasttime = (*it)->getTimeSlice();
		if ( lasttime> interval)
		{
			(*it)->modifyTimeSlice(lasttime - interval);
			qDebug() << "roomid: " << (*it)->getRoomID() << "waitime:" << (*it)->getTimeSlice();
		}
		else if (lasttime <= interval)
		{
			(*it)->modifyTimeSlice(0);
			qDebug() << "roomid: " << (*it)->getRoomID() << "waitime:" << (*it)->getTimeSlice();
			/*
			1.待完成，时间片减为0时创建信号，shceduler进行调度
			2.待完成，时间片减为0时创建信号，向数据库中写状态。
			*/
			emit waitFinish((*it)->getWaitID());
		}
		++it;
	}
}

RequestObject* queueRequestObject::getRequestObjectRoomID(int RoomID)
{
	QVector<RequestObject*>::iterator it;
	for (it = queue.begin(); it != queue.end();)
	{
		if (it == nullptr)
		{
			++it;
			continue;
		}
		else if ((*it)->getRoomID() == RoomID)
		{
			return *it;
		}
		else {
			++it;
		}
	}
	return nullptr;
}

RequestObject* queueRequestObject::getRequestObjectWaitID(int WaitID)
{
	QVector<RequestObject*>::iterator it;
	for (it = queue.begin(); it != queue.end();)
	{
		if (it == nullptr)
		{
			++it;
			continue;
		}
		else if ((*it)->getWaitID() == WaitID)
		{
			return *it;
		}
		else {
			++it;
		}
	}
	return nullptr;
}

RequestObject* queueRequestObject::getRequestObjectShortest()
{
	QVector<RequestObject*>::iterator it;
	int shortesttime = 120;
	int requestID;
	for (it = queue.begin(); it != queue.end();)
	{
		if (it == nullptr)
		{
			++it;
			continue;
		}
		else if ((*it)->getTimeSlice()<=shortesttime)
		{
			shortesttime = (*it)->getTimeSlice();
			requestID = (*it)->getWaitID();
			++it;
		}
		else {
			++it;
		}
	}
	if (requestID != 0)
	{
		return this->getRequestObjectWaitID(requestID);
	}
	else
	{
		return nullptr;
	}
}

RequestObject* queueRequestObject::getRequestObjectShortest(int FanSpeed)
{
	QVector<RequestObject*>::iterator it;
	int shortesttime = 120;
	int requestID;
	for (it = queue.begin(); it != queue.end();)
	{
		if (it == nullptr)
		{
			++it;
			continue;
		}
		else if ((*it)->getTimeSlice() <= shortesttime&&(*it)->getFanSpeed()>FanSpeed)
		{
			shortesttime = (*it)->getTimeSlice();
			requestID = (*it)->getWaitID();
			++it;
		}
		else {
			++it;
		}
	}
	if (requestID != 0)
	{
		return this->getRequestObjectWaitID(requestID);
	}
	else
	{
		return nullptr;
	}
}

void queueRequestObject::deleteRequest(int WaitID)
{
	QVector<RequestObject*>::iterator it;
	for (it = queue.begin(); it != queue.end();)
	{
		if (it == nullptr)
		{
			++it;
			continue;
		}
		else if ((*it)->getWaitID() == WaitID)
		{
			queue.erase(it);
			//delete (*it);
		}
		else {
			++it;
		}
	}
}

int queueRequestObject::size()
{
	return queue.size();
}

int queueRequestObject::getawaitid()
{
	int i = 0;
	while (getRequestObjectWaitID(i + 1) != nullptr)
	{
		i++;
	}
	return i+1;
}

bool queueRequestObject::empty()
{
	return queue.empty();
}

RequestObject::RequestObject()
{
}

RequestObject::RequestObject(float defaultTemp, float defaultSpeed, int defaultmode)
{
	this->TargetTemp = defaultTemp;
	this->FanSeed = defaultSpeed;
	this->Mode = defaultmode;
	this->TimeSlice = 120;
}

RequestObject::~RequestObject()
{
}

void RequestObject::assignWait(int waitID)
{
	this->WaitID = waitID;
}

void RequestObject::startWait(int RoomID, float ctemp, float ttemp, int speed,float Fee, int Mode)
{
	this->RoomID = RoomID;
	this->currentemp = ctemp;
	this->TargetTemp = ttemp;
	this->FanSeed = speed;
	this->Fee = Fee;
	this->Mode = Mode;
	this->TimeSlice = 120;
}

float RequestObject::getFee()
{
	return this->Fee;
}

int RequestObject::getRoomID()
{
	return this->RoomID;
}

int RequestObject::getWaitID()
{
	return this->WaitID;
}

float RequestObject::getCurrenTemp()
{
	return this->currentemp;
}

float RequestObject::getTargetTemp()
{
	return this->TargetTemp;
}

int RequestObject::getFanSpeed()
{
	return this->FanSeed;
}

int RequestObject::getTimeSlice()
{
	return this->TimeSlice;
}

int RequestObject::getMode()
{
	return this->Mode;
}

void RequestObject::modifyTtemp(float ttemp)
{
	this->TargetTemp = ttemp;
}

void RequestObject::modifyFanSpeed(float speed)
{
	this->FanSeed = speed;
}

void RequestObject::modifyTimeSlice(int newtimeslice)
{
	this->TimeSlice = newtimeslice;
}

void RequestObject::updatectemp(float speed)
{
	this->currentemp = speed;
}

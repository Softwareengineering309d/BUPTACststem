#include "Scheduler.h"

Scheduler::Scheduler(QObject *parent)
	: QObject(parent)
{
	return;
}

Scheduler::~Scheduler()
{
}

int Scheduler::createServiceQueue() {
	ACsystemServer::serviceQueue = QVector<int>(); //@注：记得改成QVector<ServiceObj*>
	return 0;
}

int Scheduler::createWaitingQueue() {
	ACsystemServer::waitingQueue = QVector<int>(); //@注：记得改成QVector<RequestObj*>
	return 0;
}

int Scheduler::ready() {
	this->isReady = true;
	return 0;
}

int Scheduler::setPara(int defaultMode, float tempHighLimit, float tempLowLimit, float defaultTargetTemp, float defaultFeeRate) {
	this->defaultMode = defaultMode;
	this->tempHighLimit = tempHighLimit;
	this->tempLowLimit = tempLowLimit;
	this->defaultTargetTemp = defaultTargetTemp;
	this->defaultFeeRate = defaultFeeRate;
	return 0;
}

QVector<int> Scheduler::getRoomState() {
	QVector<int> roomStates;
	// TODO: 去DBFacade查roomState，把全部房间的状态放入roomStates。
	return roomStates;
}
#include "ACController.h"

ACController::ACController(QObject *parent)
	: QObject(parent)
{
	return;
}

ACController::~ACController()
{
}

void ACController::powerOn() {
	this->createScheduler();
	return;
}

int ACController::createScheduler() {
	ACsystemServer::scheduler.isReady = false;
	ACsystemServer::scheduler.createServiceQueue();
	ACsystemServer::scheduler.createWaitingQueue();
	return 0;
}

void ACController::sendPara(int defaultMode, float tempHighLimit, float tempLowLimit, float defaultTargetTemp, float defaultFeeRate) {
	ACsystemServer::scheduler.setPara(defaultMode, tempHighLimit, tempLowLimit, defaultTargetTemp, defaultFeeRate);
}

void ACController::startUp() {
	ACsystemServer::scheduler.ready();
}

QVector<int> ACController::checkRoomState() {
	return ACsystemServer::scheduler.getRoomState();
}
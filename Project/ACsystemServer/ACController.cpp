#include "ACController.h"



ACController::ACController(QObject *parent)
	: QObject(parent)
{
	return;
}

ACController::~ACController()
{
}

void ACController::powerOn(Scheduler* scheduler) {
	this->createScheduler(scheduler);
	return;
}



int ACController::createScheduler(Scheduler* scheduler) {
	scheduler->isReady = false;
	scheduler->createServiceQueue();
	scheduler->createWaitingQueue();
	return 0;
}

void ACController::sendPara(Scheduler* scheduler,int defaultMode, float tempHighLimit, float tempLowLimit, float defaultTargetTemp, int FanSpeed,float Feerate) {
	scheduler->setParm(defaultMode, defaultTargetTemp, FanSpeed, tempHighLimit, tempLowLimit, Feerate);
}

void ACController::startUp(Scheduler* scheduler) {
	scheduler->ready();
}

QVector<int> ACController::checkRoomState(Scheduler* scheduler) {
	return scheduler->getRoomState();
}
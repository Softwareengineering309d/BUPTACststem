#include "ACController.h"
#include "global.h"



ACController::ACController(QObject *parent)
	: QObject(parent)
{
	// 与RoomUp信号相连，以管理中央空调
	if (RoomUp == nullptr) {
		qDebug() << "test failed" << endl;
	}
	else {
		qDebug() << "test success" << endl;
	}

	return;
}

ACController::~ACController()
{
}

void ACController::connect_with_manager()
{
	connect(RoomUp, SIGNAL(signal_air_PowerOn()),
		this, SLOT(powerOn()));
	connect(RoomUp, SIGNAL(signal_air_SendPara(int, float, float, float, float)),
		this, SLOT(sendPara(int, float, float, float,MID_FANSPEED, float)));
	connect(RoomUp, SIGNAL(signal_air_StartUp()),
		this, SLOT(startUp()));
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
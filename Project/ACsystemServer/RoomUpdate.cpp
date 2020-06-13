#include "RoomUpdate.h"

RoomUpdate::RoomUpdate()
{}

RoomUpdate::~RoomUpdate()
{}

// ���·�����Ϣ
void RoomUpdate::update_server(int RoomId, int serverState)
{
    emit signal_update_server(RoomId, serverState);
}

// ����ʵʱ��Ϣ
void RoomUpdate::update_timing(int RoomId, int state, float cur_temp, float dist_temp, int speed, float fee)
{
    emit signal_update_timing(RoomId, state, cur_temp, dist_temp, speed, fee);
}

// ����Դ
void RoomUpdate::air_PowerOn()
{
    emit signal_air_PowerOn();
}

// ����
void RoomUpdate::air_SendPara(int defaultMode, float tempHighLimit, float tempLowLimit, float defaultTargetTemp, float defaultFeeRate)
{
    emit signal_air_SendPara(defaultMode, tempHighLimit, tempLowLimit, defaultTargetTemp, defaultFeeRate);
}

// ����
void RoomUpdate::air_StartUp()
{
    emit signal_air_StartUp();
}

#include "RoomUpdate.h"

RoomUpdate::RoomUpdate()
{}

RoomUpdate::~RoomUpdate()
{}

// 更新服务信息
void RoomUpdate::update_server(int RoomId, int serverState)
{
    emit signal_update_server(RoomId, serverState);
}

// 更新实时信息
void RoomUpdate::update_timing(int RoomId, int state, float cur_temp, float dist_temp, int speed, float fee)
{
    emit signal_update_timing(RoomId, state, cur_temp, dist_temp, speed, fee);
}

// 开电源
void RoomUpdate::air_PowerOn()
{
    emit signal_air_PowerOn();
}

// 参数
void RoomUpdate::air_SendPara(int defaultMode, float tempHighLimit, float tempLowLimit, float defaultTargetTemp, float defaultFeeRate)
{
    emit signal_air_SendPara(defaultMode, tempHighLimit, tempLowLimit, defaultTargetTemp, defaultFeeRate);
}

// 运行
void RoomUpdate::air_StartUp()
{
    emit signal_air_StartUp();
}

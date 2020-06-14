/*******************************************************
 * 房间更新类
 *  用于管理员UI与服务器的交互信号槽
 *  添加"global.h",直接调用RoomUpdate的public函数即可
*******************************************************/

#ifndef ROOMINFO_H
#define ROOMINFO_H

#include "QObject"

class RoomUpdate : public QObject
{
    Q_OBJECT

public:
    RoomUpdate();
    ~RoomUpdate();
    // 服务队列变化时调用，0为未在服务，1为服务中，2为等待中
    void update_server(int RoomId, int serverState);
    // 定时向用户更新信息时调用，某信息未更新时可写原值或-1

    void update_timing(int RoomId, int state, float cur_temp, float dist_temp, int speed, float fee);
    // 开电源
    void air_PowerOn( );
    // 参数
    void air_SendPara(int defaultMode, float tempHighLimit, float tempLowLimit, float defaultTargetTemp, float defaultFeeRate);
    // 运行
    void air_StartUp( );
    // 更新ui
    void update_ui(int RoomId, int state, float cur_temp, float dist_temp, int speed, float fee);

signals:
    void signal_update_server(int RoomId, int serverState);
    void signal_update_timing(int RoomId, int state, float cur_temp, float dist_temp, int speed, float fee);
    void signal_air_PowerOn();
    void signal_air_SendPara(int defaultMode, float tempHighLimit, float tempLowLimit, float defaultTargetTemp, float defaultFeeRate);
    void signal_air_StartUp();
    void signal_update_ui(int RoomId, int state, float cur_temp, float dist_temp, int speed, float fee);
};

#endif // ROOMINFO_H

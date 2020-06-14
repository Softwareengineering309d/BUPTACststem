#include "global.h"

// 全局变量在此处进行定义，在"global.h"中链接
// 要使用变量，在头文件中加入"global.h"即可

bool if_start = 0;

RoomUpdate* RoomUp = new RoomUpdate;

QVector<QString> RoomIdToAttr = { QString::fromLocal8Bit("房间号"),
    QString::fromLocal8Bit("开机状态"), QString::fromLocal8Bit("当前室温"), QString::fromLocal8Bit("目标温度"),
    QString::fromLocal8Bit("当前风速"), QString::fromLocal8Bit("累计费用"), QString::fromLocal8Bit("服务状态") };
QMap<QString, int> RoomAttrToId = {};

ACsystemServer* systemserver ;

void init_global(){
    for(int i = 0; i < RoomIdToAttr.size(); ++i){
        RoomAttrToId.insert(RoomIdToAttr[i], i);
    }
    systemserver->acController.connect_with_manager();
}

time_t convert_string_to_time_t(const std::string& time_string)
{
    struct tm tm1;
    time_t time1;
    sscanf(time_string.c_str(), "%d-%d-%d %d:%d:%d", &(tm1.tm_year), &(tm1.tm_mon), &(tm1.tm_mday), &(tm1.tm_hour), &(tm1.tm_min), &(tm1.tm_sec));

    tm1.tm_year -= 1900;
    tm1.tm_mon--;
    tm1.tm_isdst = -1;
    time1 = mktime(&tm1);

    return time1;
}

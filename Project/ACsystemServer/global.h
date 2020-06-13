#ifndef NUM_ROOM
#define NUM_ROOM 5
#endif 
#ifndef GLOBAL_H
#define GLOBAL_H

#include "QObject"
#include "QMap"
#include "QVector"
#include "RoomUpdate.h"
#include "QDebug"
#include "ACController.h"
#include "ACsystemServer.h"
#include"UIAdmin.h"
#include"UIManager.h"
#include"UIManager.h"
#include"UIOptReport.h"
#include"UIPassword.h"
#include"UIReception.h"
#include"UIServerMain.h"

//风速
#define LOW_FANSPEED 0
#define MID_FANSPEED 1
#define HIGH_FANSPEED 2
//制冷制热
#define REFRIGERATION 0
#define HEATING 1
//工作状态
#define WORKING 0
#define IDLE 1
//费率
#define FEERATE 1
//房间状态
#define CLOSE 0
#define SERVICE 1
#define WAIT 2
//操作类型
#define OPEN 0
#define CLOSE 1
#define CHANGESPEED 2
#define CHANGETEMP 3

// 房间总数，在"UIAdmin.cpp"中使用



class ACsystemServer;

// 全局变量在global.cpp中定义

// 判断中央空调是否开机
extern bool if_start;
// 房间更新的全局变量，调用函数来更新admin的UI
extern RoomUpdate* RoomUp;
// 房间属性顺序的全局变量，在"UIAdmin.cpp"中使用
extern QVector<QString> RoomIdToAttr;
extern QMap<QString, int> RoomAttrToId;

// 服务器全局变量
extern ACsystemServer* systemserver;


void init_global();
//字符串转time_t，计算时间
time_t convert_string_to_time_t(const std::string& time_string);



#endif // GLOBAL_H

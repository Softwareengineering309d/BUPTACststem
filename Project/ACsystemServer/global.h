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

//����
#define LOW_FANSPEED 0
#define MID_FANSPEED 1
#define HIGH_FANSPEED 2
//��������
#define REFRIGERATION 0
#define HEATING 1
//����״̬
#define WORKING 0
#define IDLE 1
//����
#define FEERATE 1
//����״̬
#define CLOSE 0
#define SERVICE 1
#define WAIT 2
//��������
#define OPEN 0
#define CLOSE 1
#define CHANGESPEED 2
#define CHANGETEMP 3

// ������������"UIAdmin.cpp"��ʹ��



class ACsystemServer;

// ȫ�ֱ�����global.cpp�ж���

// �ж�����յ��Ƿ񿪻�
extern bool if_start;
// ������µ�ȫ�ֱ��������ú���������admin��UI
extern RoomUpdate* RoomUp;
// ��������˳���ȫ�ֱ�������"UIAdmin.cpp"��ʹ��
extern QVector<QString> RoomIdToAttr;
extern QMap<QString, int> RoomAttrToId;

// ������ȫ�ֱ���
extern ACsystemServer* systemserver;


void init_global();
//�ַ���תtime_t������ʱ��
time_t convert_string_to_time_t(const std::string& time_string);



#endif // GLOBAL_H

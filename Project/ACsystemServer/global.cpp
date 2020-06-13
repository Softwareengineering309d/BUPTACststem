#include "global.h"

// ȫ�ֱ����ڴ˴����ж��壬��"global.h"������
// Ҫʹ�ñ�������ͷ�ļ��м���"global.h"����

bool if_start = 0;

RoomUpdate* RoomUp = new RoomUpdate;

QVector<QString> RoomIdToAttr = { QString::fromLocal8Bit("�����"),
    QString::fromLocal8Bit("����״̬"), QString::fromLocal8Bit("��ǰ����"), QString::fromLocal8Bit("Ŀ���¶�"),
    QString::fromLocal8Bit("��ǰ����"), QString::fromLocal8Bit("�ۼƷ���"), QString::fromLocal8Bit("����״̬") };
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

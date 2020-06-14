/*******************************************************
 * ���������
 *  ���ڹ���ԱUI��������Ľ����źŲ�
 *  ���"global.h",ֱ�ӵ���RoomUpdate��public��������
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
    // ������б仯ʱ���ã�0Ϊδ�ڷ���1Ϊ�����У�2Ϊ�ȴ���
    void update_server(int RoomId, int serverState);
    // ��ʱ���û�������Ϣʱ���ã�ĳ��Ϣδ����ʱ��дԭֵ��-1

    void update_timing(int RoomId, int state, float cur_temp, float dist_temp, int speed, float fee);
    // ����Դ
    void air_PowerOn( );
    // ����
    void air_SendPara(int defaultMode, float tempHighLimit, float tempLowLimit, float defaultTargetTemp, float defaultFeeRate);
    // ����
    void air_StartUp( );
    // ����ui
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

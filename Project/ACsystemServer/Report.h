/****************************************************************
 * ������
 *  ��ѡ�����ɱ���ʱ�������µı�����
 *  �������ݣ�������ϢInfo����������Data
 *      ������Ϣ������ID���б� ��ʼʱ�䣬����ʱ�䡢ͼ������
 *      �������ݣ�key��Ϊ���ڻ򷿼�ţ������ش��������ȴ�����(�굥����)��
 *          ���´������������������ʱ�����ܷ���
 *      ���б���Ϊ����ͼ��ʽʱ����������Ϊ��ͬ���ڵ��б���Datas[date1]
 *          Ϊ��ͼ��ʽʱ����������Ϊ��ͬ������б���Datas[room1]
****************************************************************/

#pragma once

#include <QString>
#include <QVector>
using namespace std;

struct RoomInfo{
    QVector<int> rooms;
    QString start_date;
    QString end_date;

};

struct RoomData {
    int data_key; // "RoomID"��"����ʼ���ڿ�ʼ�ĵڼ���"
    int switch_n; // 
    int dispatch_n; //
    int temp_n; //
    int wind_n; //
    int total_time; //
    int fee; //

};

class Report {
public:
    Report(QVector<int> IDs = {}, QString s_d = "", QString e_d = "");
    ~Report();
    void add_info(QVector<int> IDs, QString s_d, QString e_d);
    void add_data(QVector<RoomData> data);
    void query();
    void print();
private:
    RoomInfo info;
    QVector<RoomData> data;
};

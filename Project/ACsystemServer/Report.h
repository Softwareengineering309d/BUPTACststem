/****************************************************************
 * 报表类
 *  当选择生成报表时，创建新的报表类
 *  报表内容：报表信息Info，报表数据Data
 *      报表信息：房间ID的列表， 起始时间，结束时间、图表类型
 *      报表数据：key（为日期或房间号）、开关次数、调度次数、(详单条数)、
 *          调温次数、调风次数、请求时长、总费用
 *      其中报表为折线图形式时，报表数据为不同日期的列表，即Datas[date1]
 *          为饼图形式时，报表数据为不同房间的列表，即Datas[room1]
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
    int data_key; // "RoomID"或"从起始日期开始的第几天"
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

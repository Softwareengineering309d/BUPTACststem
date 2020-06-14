#include "Report.h"
#include <QtCharts/qchart.h>

Report::Report(QVector<int> IDs, QString s_d, QString e_d){
    add_info(IDs, s_d, e_d);
    query();
}

Report::~Report(){

}


void Report::add_info(QVector<int> IDs, QString s_d, QString e_d) {
    this->info.rooms = IDs;
    this->info.start_date = s_d;
    this->info.end_date = e_d;
}

void Report::add_data(QVector<RoomData> data) {
    this->data = data;
}

void Report::query(){
    //////////////////////// �˴�Ӧ�������ݿ⺯�����õ�����
    /// QVector<RoomData> query(RoomInfo);
    QVector<RoomData> now;
    add_data(now);
}

void Report::print(){
    // ʹ��QTChart���ͼ��
}

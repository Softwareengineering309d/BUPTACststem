#include "UIAdmin.h"
#include "ui_UIAdmin.h"
#include "QTableWidget"
#include "math.h"
#include "QDebug"

Admin::Admin(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Admin)
{
    ui->setupUi(this);

    // 判断中央空调是否开机
    if(if_start){ // 开机，则直接进入监视界面
        ui->Pages->setCurrentWidget(ui->showPage);
        init_rooms();
    }
    else{         // 不开机，进入调参界面
        // 中央空调打开电源
        RoomUp->air_PowerOn();
        ui->Pages->setCurrentWidget(ui->startPage);
    }
}

Admin::~Admin()
{
    delete ui;
}


// 对于所有房间的框架初始化
void Admin::init_rooms()
{
    // 初始化房间信息，使用表格展示
    int rows = sqrt(NUM_ROOM);
    int columns = (NUM_ROOM + rows - 1)/rows;
    for(int i = 0; i < rows; ++i)
    for(int j = 0; j < columns; ++j){
        //对于每个单元格
        // 此处从0开始，若从1开始，需+1
        int roomID = i*columns + j;
        if(roomID > NUM_ROOM) break;
        init_room(roomID);
        ui->roomsGrid->addWidget(T[roomID],i,j);
    }

    // 使用信号槽，以更新房间信息
    //connect(RoomUp,SIGNAL(signal_update_server(int, int)),
     //       this,SLOT(update_server(int, int)));
    connect(RoomUp,SIGNAL(signal_update_ui(int, int, float, float, int, float)),
            this,SLOT(update_timing(int, int, float, float, int, float)));

    qDebug() << "create rooms success.";

    ////////////////////此处不应该发信号，只是为了测试UI效果
    //RoomUp->update_server(2, 1);
    //RoomUp->update_timing(2, 1, 23.2, 24.1, 2, 10.5);
}

// 对于单个房间的表格初始化
void Admin::init_room(int roomID)
{

    // 2列分别为属性名与内容，7行表示共有7个属性
    T[roomID] = new QTableWidget;
    T[roomID]->setColumnCount(2);
    T[roomID]->setRowCount(7);
    // 房间号
    for(int i = 0; i < RoomIdToAttr.size(); ++i){
        T[roomID]->setItem(i,0,new QTableWidgetItem(RoomIdToAttr[i]));
        T[roomID]->setItem(i,1,new QTableWidgetItem(QString::number(-1)));
    }
    QTableWidgetItem* item = T[roomID]->item(RoomAttrToId[QString::fromLocal8Bit("房间号")], 1);
    item->setText(QString::number(roomID));

}

// 调参完毕，中央空调运转
void Admin::on_ok_manager_clicked()
{
    qDebug() << "hello??" << endl;
    int defaultMode = ui->modeBox->currentText() == "制冷" ? 0 : 1;
    float tempHighLimit = ui->highTempBox->value();
    float tempLowLimit = ui->lowTempBox->value();
    float defaultTargetTemp = ui->targrtTempBox->value();
    float defaultFeeRate = ui->feeRateBox->value();
    qDebug() << "parv get success" << endl;
    RoomUp->air_SendPara(defaultMode, tempHighLimit, tempLowLimit, defaultTargetTemp, defaultFeeRate);

    // 开机进入监视页面
    RoomUp->air_StartUp();
    ui->Pages->setCurrentWidget(ui->showPage);
    init_rooms();

    qDebug() << "Start Up success" << endl;
}


// 根据房间ID，更新UI
void Admin::update_server(int RoomId, int serverState)
{
    qDebug() << "update server::" << RoomId << " serverState" << serverState << endl;
    if(serverState != -1){
        QTableWidgetItem* item = T[RoomId]->item(RoomAttrToId[QString::fromLocal8Bit("服务状态")], 1);
        item->setText(QString::number(serverState));
    }
}

// 根据房间ID，更新UI
void Admin::update_timing(int RoomId, int state, float cur_temp, float dist_temp, int speed, float fee)
{
    qDebug() << "update timing::"  << RoomId << " State:" << state << " cur_temp:"
             << cur_temp << " dist_temp:" << dist_temp << " speed:" << speed << " fee:" << fee << endl;

    if(state != -1){
        QTableWidgetItem* item = T[RoomId]->item(RoomAttrToId[QString::fromLocal8Bit("开机状态")], 1);
        item->setText(QString::number(state));
    }
    if(cur_temp != -1){
        QTableWidgetItem* item = T[RoomId]->item(RoomAttrToId[QString::fromLocal8Bit("当前室温")], 1);
        item->setText(QString::number(cur_temp));
    }
    if(dist_temp != -1){
        QTableWidgetItem* item = T[RoomId]->item(RoomAttrToId[QString::fromLocal8Bit("目标温度")], 1);
        item->setText(QString::number(dist_temp));
    }
    if(speed != -1){
        QTableWidgetItem* item = T[RoomId]->item(RoomAttrToId[QString::fromLocal8Bit("当前风速")], 1);
        item->setText(QString::number(speed));
    }
    if(fee != -1){
        QTableWidgetItem* item = T[RoomId]->item(RoomAttrToId[QString::fromLocal8Bit("累计费用")], 1);
        item->setText(QString::number(fee));
    }
}

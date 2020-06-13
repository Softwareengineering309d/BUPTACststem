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

    // �ж�����յ��Ƿ񿪻�
    if(if_start){ // ��������ֱ�ӽ�����ӽ���
        ui->Pages->setCurrentWidget(ui->showPage);
        init_rooms();
    }
    else{         // ��������������ν���
        // ����յ��򿪵�Դ
        RoomUp->air_PowerOn();
        ui->Pages->setCurrentWidget(ui->startPage);
    }
}

Admin::~Admin()
{
    delete ui;
}


// �������з���Ŀ�ܳ�ʼ��
void Admin::init_rooms()
{
    // ��ʼ��������Ϣ��ʹ�ñ��չʾ
    int rows = sqrt(NUM_ROOM);
    int columns = (NUM_ROOM + rows - 1)/rows;
    for(int i = 0; i < rows; ++i)
    for(int j = 0; j < columns; ++j){
        //����ÿ����Ԫ��
        // �˴���0��ʼ������1��ʼ����+1
        int roomID = i*columns + j;
        if(roomID > NUM_ROOM) break;
        init_room(roomID);
        ui->roomsGrid->addWidget(T[roomID],i,j);
    }

    // ʹ���źŲۣ��Ը��·�����Ϣ
    //connect(RoomUp,SIGNAL(signal_update_server(int, int)),
     //       this,SLOT(update_server(int, int)));
    connect(RoomUp,SIGNAL(signal_update_ui(int, int, float, float, int, float)),
            this,SLOT(update_timing(int, int, float, float, int, float)));

    qDebug() << "create rooms success.";

    ////////////////////�˴���Ӧ�÷��źţ�ֻ��Ϊ�˲���UIЧ��
    //RoomUp->update_server(2, 1);
    //RoomUp->update_timing(2, 1, 23.2, 24.1, 2, 10.5);
}

// ���ڵ�������ı���ʼ��
void Admin::init_room(int roomID)
{

    // 2�зֱ�Ϊ�����������ݣ�7�б�ʾ����7������
    T[roomID] = new QTableWidget;
    T[roomID]->setColumnCount(2);
    T[roomID]->setRowCount(7);
    // �����
    for(int i = 0; i < RoomIdToAttr.size(); ++i){
        T[roomID]->setItem(i,0,new QTableWidgetItem(RoomIdToAttr[i]));
        T[roomID]->setItem(i,1,new QTableWidgetItem(QString::number(-1)));
    }
    QTableWidgetItem* item = T[roomID]->item(RoomAttrToId[QString::fromLocal8Bit("�����")], 1);
    item->setText(QString::number(roomID));

}

// ������ϣ�����յ���ת
void Admin::on_ok_manager_clicked()
{
    qDebug() << "hello??" << endl;
    int defaultMode = ui->modeBox->currentText() == "����" ? 0 : 1;
    float tempHighLimit = ui->highTempBox->value();
    float tempLowLimit = ui->lowTempBox->value();
    float defaultTargetTemp = ui->targrtTempBox->value();
    float defaultFeeRate = ui->feeRateBox->value();
    qDebug() << "parv get success" << endl;
    RoomUp->air_SendPara(defaultMode, tempHighLimit, tempLowLimit, defaultTargetTemp, defaultFeeRate);

    // �����������ҳ��
    RoomUp->air_StartUp();
    ui->Pages->setCurrentWidget(ui->showPage);
    init_rooms();

    qDebug() << "Start Up success" << endl;
}


// ���ݷ���ID������UI
void Admin::update_server(int RoomId, int serverState)
{
    qDebug() << "update server::" << RoomId << " serverState" << serverState << endl;
    if(serverState != -1){
        QTableWidgetItem* item = T[RoomId]->item(RoomAttrToId[QString::fromLocal8Bit("����״̬")], 1);
        item->setText(QString::number(serverState));
    }
}

// ���ݷ���ID������UI
void Admin::update_timing(int RoomId, int state, float cur_temp, float dist_temp, int speed, float fee)
{
    qDebug() << "update timing::"  << RoomId << " State:" << state << " cur_temp:"
             << cur_temp << " dist_temp:" << dist_temp << " speed:" << speed << " fee:" << fee << endl;

    if(state != -1){
        QTableWidgetItem* item = T[RoomId]->item(RoomAttrToId[QString::fromLocal8Bit("����״̬")], 1);
        item->setText(QString::number(state));
    }
    if(cur_temp != -1){
        QTableWidgetItem* item = T[RoomId]->item(RoomAttrToId[QString::fromLocal8Bit("��ǰ����")], 1);
        item->setText(QString::number(cur_temp));
    }
    if(dist_temp != -1){
        QTableWidgetItem* item = T[RoomId]->item(RoomAttrToId[QString::fromLocal8Bit("Ŀ���¶�")], 1);
        item->setText(QString::number(dist_temp));
    }
    if(speed != -1){
        QTableWidgetItem* item = T[RoomId]->item(RoomAttrToId[QString::fromLocal8Bit("��ǰ����")], 1);
        item->setText(QString::number(speed));
    }
    if(fee != -1){
        QTableWidgetItem* item = T[RoomId]->item(RoomAttrToId[QString::fromLocal8Bit("�ۼƷ���")], 1);
        item->setText(QString::number(fee));
    }
}

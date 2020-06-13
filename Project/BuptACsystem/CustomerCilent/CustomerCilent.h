#pragma once

#include <QtWidgets/QMainWindow>
#include <QtNetwork/QTcpsocket.h>
#include "ui_CustomerCilent.h"
#include "ClientTcpSocket.h"
#include <qpushbutton.h>
#pragma execution_character_set("utf-8")

#include<QTimer>
#include<QMessageBox>
#define TIMEOUT (60*1000)//一分钟超时一次

class CustomerCilent : public QMainWindow
{
    Q_OBJECT

public:
    CustomerCilent(QWidget *parent = Q_NULLPTR);
    ClientTcpSocket* socket;
    QTimer* timer;//计时器
private slots:
    void socketReadData();
    void socketDisconnect();

signals:
    //开机
    void turnon();
    //调温
    void change_temp();
    //调风
    void change_fan();
    //关机
    void turnoff();

private:
    Ui::CustomerCilentClass ui;
    QString serverIP = "127.0.0.1";
    quint16 serverPort = 8888;

    int roomid;
    float initial_temp;//初始温度
    float current_temp;//当前温度（根据模式判断是否设置目标温度合法）
    float target_temp;//目标温度
    int target_fan;//目标风速
    float fee;//费用

    bool mode;//模式，0制冷 1制热
    float low_temp;//范围，low
    float high_temp;//范围，high

    bool activiate_flag;//回温过程中的标志位
    bool re_turnon_flag;//是否已发送开机请求
};

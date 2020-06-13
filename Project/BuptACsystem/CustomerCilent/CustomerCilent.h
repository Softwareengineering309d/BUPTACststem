#pragma once

#include <QtWidgets/QMainWindow>
#include <QtNetwork/QTcpsocket.h>
#include "ui_CustomerCilent.h"
#include "ClientTcpSocket.h"
#include <qpushbutton.h>
#pragma execution_character_set("utf-8")

#include<QTimer>
#include<QMessageBox>
#define TIMEOUT (60*1000)//һ���ӳ�ʱһ��

class CustomerCilent : public QMainWindow
{
    Q_OBJECT

public:
    CustomerCilent(QWidget *parent = Q_NULLPTR);
    ClientTcpSocket* socket;
    QTimer* timer;//��ʱ��
private slots:
    void socketReadData();
    void socketDisconnect();

signals:
    //����
    void turnon();
    //����
    void change_temp();
    //����
    void change_fan();
    //�ػ�
    void turnoff();

private:
    Ui::CustomerCilentClass ui;
    QString serverIP = "127.0.0.1";
    quint16 serverPort = 8888;

    int roomid;
    float initial_temp;//��ʼ�¶�
    float current_temp;//��ǰ�¶ȣ�����ģʽ�ж��Ƿ�����Ŀ���¶ȺϷ���
    float target_temp;//Ŀ���¶�
    int target_fan;//Ŀ�����
    float fee;//����

    bool mode;//ģʽ��0���� 1����
    float low_temp;//��Χ��low
    float high_temp;//��Χ��high

    bool activiate_flag;//���¹����еı�־λ
    bool re_turnon_flag;//�Ƿ��ѷ��Ϳ�������
};

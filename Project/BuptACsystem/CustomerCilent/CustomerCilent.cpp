#include "CustomerCilent.h"
#include <random>
#include <time.h>

QString to_fan(int fanspeed)
{
    QString fan;
    switch (fanspeed) 
    {
    case 0:
        fan = "low";
        break;
    case 1:
        fan = "mid";
        break;
    case 2:
        fan = "high";
        break;
    }
    return fan;
}

CustomerCilent::CustomerCilent(QWidget* parent)
    : QMainWindow(parent)
{
    activiate_flag = false;
    re_turnon_flag = false;
    timer = new QTimer;

   // QString butname1 = "��һ����ť";
    ui.setupUi(this);
    //ui.pushButton->setText(butname1);
    setWindowTitle("�����ؿյ������");
    //����button��Ч
    ui.change_temp_button->setEnabled(false);
    ui.change_fan_button->setEnabled(false);
    ui.turnoff_button->setEnabled(false);

    socket = new ClientTcpSocket(this);

    socket->abort();
    socket->connectToHost(serverIP, serverPort);

    if (!socket->waitForConnected()) {
        qDebug() << "Connect Failed !";
        return;
    }
    qDebug() << "Connect Successfully!";

    /**********************************
    * ClientTcpSocketʹ��˵���� ��׿
    * 1. �ӷ������յ���Ϣ��
    * �ӷ�������������Ϣ�Ѿ����յ���json��תΪ����Ϣ������Ϣ���ڴ������Ϣ�Ĳۺ����ϼ��ɡ�
    * ʾ����
    *     connect(socket, ClientTcpSocket::turnOnAirConditionerOK, this, [=](float defaultTemp, int defaultFanSpeed) {
    *           //�յ��յ�����OK��Ϣʱ�Ĵ���
    *     })��
    * 2. ������������Ϣ��
    * ������������Ϣʱֱ�ӵ���socket�ļ���public�������ɡ�
    * ʾ����
    *   ��Ҫ��������˷��Ϳͻ��˿����յ�ʱ�������к������ɣ�
    *       socket->turnOnAirConditioner(roomID);
    **********************************/

    //����//turnOnAirConditioner(roomid,initial_temp)
    connect(ui.turnon_button, &QPushButton::clicked, this, [=] {
        roomid = ui.roomid_text->toPlainText().toInt();//���id
        initial_temp = ui.initial_temp_text->toPlainText().toFloat();//���initial
        current_temp = initial_temp;//��ǰ�¶�=initial
        ui.show_current_temp->setPlainText(QString("%1").arg(current_temp));//show
        socket->turnOnAirConditioner(roomid);//��������
        
        //debug
        //ui.turnon_button->setEnabled(false);//disable����button
        ////����button��Ч
        //ui.change_temp_button->setEnabled(true);
        //ui.change_fan_button->setEnabled(true);
        //ui.turnoff_button->setEnabled(true);

        //roomid��initialtime�����
        qDebug() << "Customer: turn on AC; roomid:"<<roomid<<"; temp:"<<initial_temp;
        });

//��ack��turnOnAirConditionerOK(temp,fan,mode)
//ui��ʾĬ�ϲ�����
    connect(socket, &ClientTcpSocket::turnOnAirConditionerOK, this, [=](float Temp, int FanSpeed,bool m) {
                //�յ��յ�����OK��Ϣʱ�Ĵ���  
       //��ģʽ��������¶ȷ�Χ
    //mode
        mode = m;//ģʽ
        if (mode == 0)//����
        {
            low_temp = 18;
            high_temp = 28;
        }
        else//����
        {
            low_temp = 18;
            high_temp = 25;
        }
    
    //ui��ʾĬ�ϲ���
        ui.show_target_temp->setPlainText(QString("%1").arg(Temp));//�¶�
        ui.show_target_fan->setPlainText(to_fan(FanSpeed));//012���и���

        ui.turnon_button->setEnabled(false);//disable����button
        //����button��Ч
        ui.change_temp_button->setEnabled(true);
        ui.change_fan_button->setEnabled(true);
        ui.turnoff_button->setEnabled(true);
    
        qDebug() << "Customer: AC turn on ok; defaulttemp"<<Temp<<"; defaultFanSpeed:"<<FanSpeed<<"; mode:"<<m;
     });

//����������fee
//heartBeat(fee,temp)--->
    connect(socket, &ClientTcpSocket::heartBeat, this, [=](float fee, float temp) {
        this->fee = fee;//���·���
        current_temp = temp;//���µ�ǰ�¶�
        ui.show_fee->clear();
        ui.show_fee->setPlainText(QString("%1").arg(fee));//show����
        ui.show_current_temp->clear();
        ui.show_current_temp->setPlainText(QString("%1").arg(temp));//show��ǰ�¶�
        qDebug() << "Customer: update fee&temp; fee:"<<fee<<"; temp:"<<temp;
        });

//�յ� ���ڷ���״̬serviceOn(int)--->
    connect(socket,&ClientTcpSocket::serviceOn, this, [=](int) {
         //ֹͣ��ʱ��������ʱ��
         timer->stop();
         re_turnon_flag = false;
         activiate_flag = false;//��Ӧ���û�0
         
         //��ack501 ��ǰ�¶�serviceOnBack(id,temp,bool?)
         socket->serviceOnBack(roomid, current_temp, true);

         qDebug() << "Customer: service on with ack502 and temp:"<<current_temp;
         qDebug() << "Customer: timer stop";
     });


//��ռ�����ù�

//����
//changeTemp(id,temp)
    connect(ui.change_temp_button, &QPushButton::clicked, this, [=] {
        //�жϺϷ���
        float temp = ui.input_target_temp->toPlainText().toFloat();//�����¶�
        //��Χ
        if(temp>high_temp||temp<low_temp|| (mode == 0 && temp > current_temp) || (mode == 1 && temp < current_temp))
        {
            //������������¶�
            qDebug() << "low-high:" << low_temp << "-" << high_temp;
            QMessageBox::critical(NULL, "ERROR", "Invalid Temperature!", QMessageBox::Ok);
        }
         else
        {
            target_temp = temp;//�޸�Ŀ���¶�
            socket->changeTemp(roomid, target_temp);
            ui.show_target_temp->clear();
            ui.show_target_temp->setPlainText(QString("%1").arg(target_temp));
        }

        //���ܶԴ����������
        ui.input_target_temp->clear();

        qDebug() << "Customer: change temp to"<<temp;
     });


//����
//changeFanSpeed(id,speed)
    connect(ui.change_fan_button,&QPushButton::clicked,this,[=]{
    if(ui.input_fan->currentIndex()==0)//high
    target_fan = 2;
    else if(ui.input_fan->currentIndex() == 1)//mid
    target_fan=1;
    else//low
    target_fan=0;

    //������
    socket->changeFanSpeed(roomid,target_fan);
    ui.show_target_fan->clear();
    ui.show_target_fan->setPlainText(to_fan(target_fan));

    qDebug() << "Customer: change fan to"<<target_fan;
    
    });

//�����¶�
//reachTargetTempStop()---->�������£�0.5/min
    connect(socket, &ClientTcpSocket::reachTargetTempStop, this, [=]{
        //����reachTargetTempStopBack(true)ȷ��
        socket->reachTargetTempStopBack(true);
        qDebug() << "Customer: reach temp";

        //��־λ��λ
        activiate_flag = false;
        re_turnon_flag = false;
        //�¶ȵ����ʱ������;ÿ��1���Ӷ�ʱ����ʱһ��
        timer->start(TIMEOUT);

        int plus_div = (mode == 0) ? 1 : -1;//���� 1������ -1
        connect(timer, &QTimer::timeout, this, [=] {
             //δ�����ʼ�¶ȣ���������
            qDebug() << "Customer: recover-temp  mode: " << mode << "; current_temp: " << current_temp << ": initial_temp: " << initial_temp;
            qDebug() << "Customer: befor----activiate_flag:" << activiate_flag << "; re_turnon_flag:" << re_turnon_flag;
             if ((mode == 0 && (current_temp < initial_temp)) || (mode == 1 && (current_temp  > initial_temp)))
             {
                 //temp+/-0.5
                current_temp += (0.5 * plus_div);
                 if ((mode == 0 && current_temp > initial_temp)|| (mode == 1 && current_temp < initial_temp))
                 current_temp = initial_temp;//����

                //���ж��Ƿ��ǵڶ��Σ�+1��
                 if (re_turnon_flag == false && activiate_flag == true)
                     //δ�����������Լ��ӹ�һ��0.5
                     //-->���ǵ�һ��+1��
                 {
                     socket->turnOnAirConditioner(roomid);
                     re_turnon_flag = true;//��ʾ�Ѿ��ط�
                     //���޸�activiate
                     qDebug() << "Customer: should re-turnon AC";
                 }
                //��activiate
                  if (activiate_flag == false)//�״�+0.5
                  {
                     activiate_flag = true;
                     qDebug() << "Customer: first +/-0.5";
                  }
                  ui.show_current_temp->clear();
                  ui.show_current_temp->setPlainText(QString("%1").arg(current_temp));
                  qDebug() << "Customer: +/-0.5 fini";
             }
             //�ص�Ŀ���¶�,����Ҫ�ٻ���

             //�����ж�ʱ���͵�ǰ�¶�
             socket->keepAlive(current_temp);
             qDebug() << "Customer: alive current_temp: " << current_temp;
             qDebug() << "Customer: after----activiate_flag:" << activiate_flag << "; re_turnon_flag:" << re_turnon_flag;
         });
     //��������ʼ�ṩ���񣬲�ֹͣtimer
    });


//�ػ�
//closeAirConditioner(id)
    connect(ui.turnoff_button,&QPushButton::clicked,this,[=]{
        socket->closeAirConditioner(roomid);
        ui.turnoff_button->setEnabled(false);
        current_temp=initial_temp;//�ػ��ظ���ʼ�¶�
        //����turnon
        ui.turnon_button->setEnabled(true);
        ui.show_current_temp->clear();
        ui.show_current_temp->setPlainText(QString("%1").arg(current_temp));

        //����button��Ч
        ui.change_temp_button->setEnabled(false);
        ui.change_fan_button->setEnabled(false);
        ui.turnoff_button->setEnabled(false);

        qDebug() << "Customer: turn off AC";
    });

//��ʾ��Ϣ�ǣ���������
        //�յ�����ERROR
    //void turnOnAirConditionerError();
    ////�޸��¶�OK
    //void changeTempOK();
    ////�޸��¶�Error
    //void changeTempError();
    ////�޸ķ���OK
    //void changeFanSpeedOK();
    ////�޸ķ���Error
    //void changeFanSpeedError();
    ////�رտյ�OK
    //void closeAirConditionerOK();
    ////�رտյ�Error
    //void closeAirConditionerError();
    connect(socket, &ClientTcpSocket::turnOnAirConditionerError, this, [=]{
        QMessageBox::critical(NULL, "ERROR","Turn on Air-Conditioner Error!",QMessageBox::Ok);
        ui.turnon_button->setEnabled(true);
    });
    connect(socket, &ClientTcpSocket::changeTempOK, this, [=] {
        QMessageBox::information(NULL, "Note", "Change Temperature OK");
    });
    connect(socket, &ClientTcpSocket::changeTempError, this, [=] {
        QMessageBox::critical(NULL, "ERROR", "Change Temperature ERROR!", QMessageBox::Ok);
        });
    connect(socket, &ClientTcpSocket::changeFanSpeedOK, this, [=] {
        QMessageBox::information(NULL, "Note", "Change Fan Speed OK");
        });
    connect(socket, &ClientTcpSocket::changeFanSpeedError, this, [=] {
        QMessageBox::critical(NULL, "ERROR", "Change Fan Speed ERROR!", QMessageBox::Ok);
        });
    connect(socket, &ClientTcpSocket::closeAirConditionerOK, this, [=] {
        QMessageBox::information(NULL, "Note", "Close Air-Conditional OK");
        });
    connect(socket, &ClientTcpSocket::closeAirConditionerError, this, [=] {
        QMessageBox::critical(NULL, "ERROR", "Close Air-Conditioner ERROR!", QMessageBox::Ok);
        });


    //����

    ////���Դ���
    //std::default_random_engine e;
    //e.seed(time(0));
    //int roomid = e() % 10;
    //socket->turnOnAirConditioner(roomid);
    //socket->changeFanSpeed(roomid, 3);
    //socket->changeTemp(roomid, 26.5);

    //Sleep(10000);

    //socket->closeAirConditioner(roomid);
    //socket->serviceOnBack(roomid, 28.9, true);
    //socket->serviceOnBack(roomid);
    //socket->reachTargetTempStopBack(true);
    //socket->preemptedStopBack(true);

   /*
    //���Դ���
    connect(socket, &QTcpSocket::readyRead, this, &CustomerCilent::socketReadData);
    connect(socket, &QTcpSocket::disconnected, this, &CustomerCilent::socketDisconnect);
    
    QString msg = "{\"request\":300,\"data\":{\"RoomId\":1, \"FanSpeed\":1}}";

    socket->write(msg.toUtf8().data());*/
}

void CustomerCilent::socketReadData() {
    QByteArray buffer;
    buffer = socket->readAll();
    QString str;
    if (!buffer.isEmpty()) {
        str += tr(buffer);
    }
    qDebug() << str;
    return;
}

void CustomerCilent::socketDisconnect() {
    qDebug() << "Now disconnected.";
    return;
}



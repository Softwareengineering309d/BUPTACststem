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

   // QString butname1 = "第一个按钮";
    ui.setupUi(this);
    //ui.pushButton->setText(butname1);
    setWindowTitle("波普特空调服务端");
    //其他button无效
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
    * ClientTcpSocket使用说明： 李卓
    * 1. 从服务器收到消息：
    * 从服务器发来的消息已经从收到的json包转为了消息，把消息绑定在处理该消息的槽函数上即可。
    * 示例：
    *     connect(socket, ClientTcpSocket::turnOnAirConditionerOK, this, [=](float defaultTemp, int defaultFanSpeed) {
    *           //收到空调开启OK消息时的处理
    *     })；
    * 2. 往服务器发消息：
    * 往服务器发消息时直接调用socket的几个public函数即可。
    * 示例：
    *   想要向服务器端发送客户端开启空调时调用下列函数即可：
    *       socket->turnOnAirConditioner(roomID);
    **********************************/

    //开机//turnOnAirConditioner(roomid,initial_temp)
    connect(ui.turnon_button, &QPushButton::clicked, this, [=] {
        roomid = ui.roomid_text->toPlainText().toInt();//获得id
        initial_temp = ui.initial_temp_text->toPlainText().toFloat();//获得initial
        current_temp = initial_temp;//当前温度=initial
        ui.show_current_temp->setPlainText(QString("%1").arg(current_temp));//show
        socket->turnOnAirConditioner(roomid);//开机请求
        
        //debug
        //ui.turnon_button->setEnabled(false);//disable开机button
        ////其他button有效
        //ui.change_temp_button->setEnabled(true);
        //ui.change_fan_button->setEnabled(true);
        //ui.turnoff_button->setEnabled(true);

        //roomid、initialtime不清空
        qDebug() << "Customer: turn on AC; roomid:"<<roomid<<"; temp:"<<initial_temp;
        });

//收ack，turnOnAirConditionerOK(temp,fan,mode)
//ui显示默认参数们
    connect(socket, &ClientTcpSocket::turnOnAirConditionerOK, this, [=](float Temp, int FanSpeed,bool m) {
                //收到空调开启OK消息时的处理  
       //存模式，存合理温度范围
    //mode
        mode = m;//模式
        if (mode == 0)//制冷
        {
            low_temp = 18;
            high_temp = 28;
        }
        else//制热
        {
            low_temp = 18;
            high_temp = 25;
        }
    
    //ui显示默认参数
        ui.show_target_temp->setPlainText(QString("%1").arg(Temp));//温度
        ui.show_target_fan->setPlainText(to_fan(FanSpeed));//012低中高速

        ui.turnon_button->setEnabled(false);//disable开机button
        //其他button有效
        ui.change_temp_button->setEnabled(true);
        ui.change_fan_button->setEnabled(true);
        ui.turnoff_button->setEnabled(true);
    
        qDebug() << "Customer: AC turn on ok; defaulttemp"<<Temp<<"; defaultFanSpeed:"<<FanSpeed<<"; mode:"<<m;
     });

//心跳，更新fee
//heartBeat(fee,temp)--->
    connect(socket, &ClientTcpSocket::heartBeat, this, [=](float fee, float temp) {
        this->fee = fee;//更新费用
        current_temp = temp;//更新当前温度
        ui.show_fee->clear();
        ui.show_fee->setPlainText(QString("%1").arg(fee));//show费用
        ui.show_current_temp->clear();
        ui.show_current_temp->setPlainText(QString("%1").arg(temp));//show当前温度
        qDebug() << "Customer: update fee&temp; fee:"<<fee<<"; temp:"<<temp;
        });

//收到 处于服务状态serviceOn(int)--->
    connect(socket,&ClientTcpSocket::serviceOn, this, [=](int) {
         //停止计时器（回温时）
         timer->stop();
         re_turnon_flag = false;
         activiate_flag = false;//响应，置回0
         
         //回ack501 当前温度serviceOnBack(id,temp,bool?)
         socket->serviceOnBack(roomid, current_temp, true);

         qDebug() << "Customer: service on with ack502 and temp:"<<current_temp;
         qDebug() << "Customer: timer stop";
     });


//抢占，不用管

//调温
//changeTemp(id,temp)
    connect(ui.change_temp_button, &QPushButton::clicked, this, [=] {
        //判断合法性
        float temp = ui.input_target_temp->toPlainText().toFloat();//输入温度
        //范围
        if(temp>high_temp||temp<low_temp|| (mode == 0 && temp > current_temp) || (mode == 1 && temp < current_temp))
        {
            //弹窗，错误的温度
            qDebug() << "low-high:" << low_temp << "-" << high_temp;
            QMessageBox::critical(NULL, "ERROR", "Invalid Temperature!", QMessageBox::Ok);
        }
         else
        {
            target_temp = temp;//修改目标温度
            socket->changeTemp(roomid, target_temp);
            ui.show_target_temp->clear();
            ui.show_target_temp->setPlainText(QString("%1").arg(target_temp));
        }

        //不管对错，都清空内容
        ui.input_target_temp->clear();

        qDebug() << "Customer: change temp to"<<temp;
     });


//调风
//changeFanSpeed(id,speed)
    connect(ui.change_fan_button,&QPushButton::clicked,this,[=]{
    if(ui.input_fan->currentIndex()==0)//high
    target_fan = 2;
    else if(ui.input_fan->currentIndex() == 1)//mid
    target_fan=1;
    else//low
    target_fan=0;

    //发更新
    socket->changeFanSpeed(roomid,target_fan);
    ui.show_target_fan->clear();
    ui.show_target_fan->setPlainText(to_fan(target_fan));

    qDebug() << "Customer: change fan to"<<target_fan;
    
    });

//到达温度
//reachTargetTempStop()---->启动回温，0.5/min
    connect(socket, &ClientTcpSocket::reachTargetTempStop, this, [=]{
        //发送reachTargetTempStopBack(true)确认
        socket->reachTargetTempStopBack(true);
        qDebug() << "Customer: reach temp";

        //标志位复位
        activiate_flag = false;
        re_turnon_flag = false;
        //温度到达，定时器开启;每隔1分钟定时器超时一次
        timer->start(TIMEOUT);

        int plus_div = (mode == 0) ? 1 : -1;//制冷 1；制热 -1
        connect(timer, &QTimer::timeout, this, [=] {
             //未到达初始温度，继续回温
            qDebug() << "Customer: recover-temp  mode: " << mode << "; current_temp: " << current_temp << ": initial_temp: " << initial_temp;
            qDebug() << "Customer: befor----activiate_flag:" << activiate_flag << "; re_turnon_flag:" << re_turnon_flag;
             if ((mode == 0 && (current_temp < initial_temp)) || (mode == 1 && (current_temp  > initial_temp)))
             {
                 //temp+/-0.5
                current_temp += (0.5 * plus_div);
                 if ((mode == 0 && current_temp > initial_temp)|| (mode == 1 && current_temp < initial_temp))
                 current_temp = initial_temp;//修正

                //先判断是否是第二次（+1）
                 if (re_turnon_flag == false && activiate_flag == true)
                     //未发送重启，以及加过一次0.5
                     //-->这是第一个+1度
                 {
                     socket->turnOnAirConditioner(roomid);
                     re_turnon_flag = true;//表示已经重发
                     //不修改activiate
                     qDebug() << "Customer: should re-turnon AC";
                 }
                //改activiate
                  if (activiate_flag == false)//首次+0.5
                  {
                     activiate_flag = true;
                     qDebug() << "Customer: first +/-0.5";
                  }
                  ui.show_current_temp->clear();
                  ui.show_current_temp->setPlainText(QString("%1").arg(current_temp));
                  qDebug() << "Customer: +/-0.5 fini";
             }
             //回到目标温度,不需要再回温

             //回温中定时发送当前温度
             socket->keepAlive(current_temp);
             qDebug() << "Customer: alive current_temp: " << current_temp;
             qDebug() << "Customer: after----activiate_flag:" << activiate_flag << "; re_turnon_flag:" << re_turnon_flag;
         });
     //服务器开始提供服务，才停止timer
    });


//关机
//closeAirConditioner(id)
    connect(ui.turnoff_button,&QPushButton::clicked,this,[=]{
        socket->closeAirConditioner(roomid);
        ui.turnoff_button->setEnabled(false);
        current_temp=initial_temp;//关机回复起始温度
        //可以turnon
        ui.turnon_button->setEnabled(true);
        ui.show_current_temp->clear();
        ui.show_current_temp->setPlainText(QString("%1").arg(current_temp));

        //其他button无效
        ui.change_temp_button->setEnabled(false);
        ui.change_fan_button->setEnabled(false);
        ui.turnoff_button->setEnabled(false);

        qDebug() << "Customer: turn off AC";
    });

//提示信息们，弹窗即可
        //空调开启ERROR
    //void turnOnAirConditionerError();
    ////修改温度OK
    //void changeTempOK();
    ////修改温度Error
    //void changeTempError();
    ////修改风速OK
    //void changeFanSpeedOK();
    ////修改风速Error
    //void changeFanSpeedError();
    ////关闭空调OK
    //void closeAirConditionerOK();
    ////关闭空调Error
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


    //调试

    ////测试代码
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
    //测试代码
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



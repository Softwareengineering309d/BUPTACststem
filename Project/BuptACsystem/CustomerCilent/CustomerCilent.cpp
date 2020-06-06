#include "CustomerCilent.h"
CustomerCilent::CustomerCilent(QWidget *parent)
    : QMainWindow(parent)
{
    QString butname1 = "第一个按钮";
    ui.setupUi(this);
    ui.pushButton->setText(butname1);
    setWindowTitle("波普特空调服务器");

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

    //

    ////测试代码
    //int roomid = 1;
    //socket->turnOnAirConditioner(roomid);
    //socket->changeFanSpeed(roomid, 3);
    //socket->changeTemp(roomid, 26.5);

    ////Sleep(10000);

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

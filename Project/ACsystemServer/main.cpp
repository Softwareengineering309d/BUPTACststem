#include "ACsystemServer.h"
#include"global.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ServerMain MainWindow;
    systemserver = new ACsystemServer();
    //Ò»¸ö²âÊÔ
    systemserver->scheduler.setParm(HEATING, 22, MID_FANSPEED, 25, 18, 1);
    //systemserver->scheduler.requestOn(100);
    MainWindow.show();
    return a.exec();
}

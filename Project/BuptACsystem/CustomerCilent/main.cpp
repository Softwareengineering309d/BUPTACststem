#include "CustomerCilent.h"
#include <QtWidgets/QApplication>
#include<qtextcodec.h>
#pragma execution_character_set("utf-8")
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    CustomerCilent w;
    w.show();
    return a.exec();
}

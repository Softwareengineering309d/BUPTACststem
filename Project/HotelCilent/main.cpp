#include "HotelCilent.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    HotelCilent w;
    w.show();
    return a.exec();
}

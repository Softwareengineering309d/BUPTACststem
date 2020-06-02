#include "CustomerCilent.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CustomerCilent w;
    w.show();
    return a.exec();
}

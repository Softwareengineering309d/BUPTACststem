#include "CustomerCilent.h"
CustomerCilent::CustomerCilent(QWidget *parent)
    : QMainWindow(parent)
{
    QString butname1 = "第一个按钮";
    ui.setupUi(this);
    ui.pushButton->setText(butname1);
    setWindowTitle("波普特空调服务器");
}

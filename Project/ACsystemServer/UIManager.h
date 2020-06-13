/*******************************************
 * 酒店经理UI类
 *  点击"新建报表"：选择时间与格式，得到新报表
 *  "查看报表"选择框：选择已有报表，在右侧显示
 *  每个报表可进行"打印"与"删除"操作
 *  退出页面时，默认删除所有报表
********************************************/

#ifndef UIMANAGER_H
#define UIMANAGER_H

#include <QMainWindow>
#include "Report.h"

namespace Ui {
class Manager;
}

class Manager : public QMainWindow
{
    Q_OBJECT

public:
    explicit Manager(QWidget *parent = nullptr);
    ~Manager();

private slots:

    int create_new_report();
    void on_createButton_clicked();

private:
    int report_num;
    Ui::Manager *ui;
    QVector<Report*> reports;
};

#endif // UIMANAGER_H

/*********************************************
 * 空调管理员UI类
 *  默认密码为"Admin"
 *  默认房间数目为6，在"global.h"中定义
 *  实时监测房间的运行参数：
 *      开关机、室温、目标温度、风速、费用、服务情况
 *  初始参数均为-1，需要等待更新
 *
 *  更新房间UI的方式，可见"RoomUpdate.h"
**********************************************/
#ifndef UIADMIN_H
#define UIADMIN_H
#include <QMainWindow>
#include "QTableWidget"
#include "global.h"

namespace Ui {
class Admin;
}

class Admin : public QMainWindow
{
    Q_OBJECT

public:
    explicit Admin(QWidget *parent = nullptr);
    ~Admin();

private:
    Ui::Admin *ui;
    QTableWidget* T[NUM_ROOM];

    void init_rooms();
    void init_room(int RoomId);

private slots:
    void update_server(int RoomId, int serverState);
    void update_timing(int RoomId, int state, float cur_temp, float dist_temp, int speed, float fee);
    void on_ok_manager_clicked();
};

#endif // UIADMIN_H

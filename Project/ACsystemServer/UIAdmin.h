/*********************************************
 * �յ�����ԱUI��
 *  Ĭ������Ϊ"Admin"
 *  Ĭ�Ϸ�����ĿΪ6����"global.h"�ж���
 *  ʵʱ��ⷿ������в�����
 *      ���ػ������¡�Ŀ���¶ȡ����١����á��������
 *  ��ʼ������Ϊ-1����Ҫ�ȴ�����
 *
 *  ���·���UI�ķ�ʽ���ɼ�"RoomUpdate.h"
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

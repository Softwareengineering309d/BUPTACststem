/*******************************************
 * �Ƶ꾭��UI��
 *  ���"�½�����"��ѡ��ʱ�����ʽ���õ��±���
 *  "�鿴����"ѡ���ѡ�����б������Ҳ���ʾ
 *  ÿ������ɽ���"��ӡ"��"ɾ��"����
 *  �˳�ҳ��ʱ��Ĭ��ɾ�����б���
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

/*******************************************
 * ��������UI��
 *  ��ѡ��"����"��"ǰ̨"��"����Ա"�������
 *  ������ȷ����󣬽�����ӦUIҳ��
 *  ��ҳ��Ϊ��ģ̬��������ͬʱ���������ݵ�ҳ��
********************************************/

#ifndef UISERVERMAIN_H
#define UISERVERMAIN_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class ServerMain; }
QT_END_NAMESPACE

class ServerMain : public QMainWindow
{
    Q_OBJECT

public:
    ServerMain(QWidget *parent = nullptr);
    ~ServerMain();

private slots:
    void on_managerButton_clicked();

    void on_receptionButton_clicked();

    void on_adminButton_clicked();

    bool check_password(QString kind);

    void get_password(QString input_pwd);

private:
    Ui::ServerMain *ui;
    QString pwd;
};
#endif // UISERVERMAIN_H

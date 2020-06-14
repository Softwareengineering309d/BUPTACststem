/*******************************************
 * 服务器总UI类
 *  可选择"经理"、"前台"、"管理员"三种身份
 *  输入正确密码后，进入相应UI页面
 *  新页面为非模态，即可以同时开启多个身份的页面
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

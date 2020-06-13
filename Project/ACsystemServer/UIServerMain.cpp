#include "UIServerMain.h"
#include "ui_UIServerMain.h"
#include "UIManager.h"
#include "UIReception.h"
#include "UIAdmin.h"
#include "UIPassword.h"
#include <QMessageBox>
#include <QDebug>

ServerMain::ServerMain(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ServerMain)
{
    ui->setupUi(this);
}

ServerMain::~ServerMain()
{
    delete ui;
}


// 经理界面
void ServerMain::on_managerButton_clicked()
{
    if(check_password("Manager")){
        Manager* M = new Manager();
        M->show();
    }
    pwd = "";
}

// 前台界面
void ServerMain::on_receptionButton_clicked()
{
    if(check_password("Reception")){
        Reception* R = new Reception();
        R->show();
    }
}

// 管理员界面
void ServerMain::on_adminButton_clicked()
{
    if(check_password("Admin")){
        Admin* A = new Admin();
        A->show();
    }
}

// 检查密码
bool ServerMain::check_password(QString kind)
{
    pwd = "";
    password* P = new password();
    // 当在密码界面点击“确认”时，关联的pwd存储
    connect(P,SIGNAL(post_password(QString)),this,SLOT(get_password(QString)));
    P->setModal(true);
    P->exec();

    qDebug() << pwd << endl;
    /*******************pwd应等于数据库中存储的默认密码，需要调用相关数据库函数，此处先设为kind以便调试*******************************/
    if(pwd == kind){
        return 1;
    }
    else{
        QMessageBox::critical(NULL, "critical", QString::fromLocal8Bit("请输入正确") + kind + QString::fromLocal8Bit("密码"), QMessageBox::Yes);
        return 0;
    }
}

// 使用信号槽获取输入的密码
void ServerMain::get_password(QString input_pwd)
{
    pwd = input_pwd;
}

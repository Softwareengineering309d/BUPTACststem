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


// �������
void ServerMain::on_managerButton_clicked()
{
    if(check_password("Manager")){
        Manager* M = new Manager();
        M->show();
    }
    pwd = "";
}

// ǰ̨����
void ServerMain::on_receptionButton_clicked()
{
    if(check_password("Reception")){
        Reception* R = new Reception();
        R->show();
    }
}

// ����Ա����
void ServerMain::on_adminButton_clicked()
{
    if(check_password("Admin")){
        Admin* A = new Admin();
        A->show();
    }
}

// �������
bool ServerMain::check_password(QString kind)
{
    pwd = "";
    password* P = new password();
    // ���������������ȷ�ϡ�ʱ��������pwd�洢
    connect(P,SIGNAL(post_password(QString)),this,SLOT(get_password(QString)));
    P->setModal(true);
    P->exec();

    qDebug() << pwd << endl;
    /*******************pwdӦ�������ݿ��д洢��Ĭ�����룬��Ҫ����������ݿ⺯�����˴�����Ϊkind�Ա����*******************************/
    if(pwd == kind){
        return 1;
    }
    else{
        QMessageBox::critical(NULL, "critical", QString::fromLocal8Bit("��������ȷ") + kind + QString::fromLocal8Bit("����"), QMessageBox::Yes);
        return 0;
    }
}

// ʹ���źŲۻ�ȡ���������
void ServerMain::get_password(QString input_pwd)
{
    pwd = input_pwd;
}

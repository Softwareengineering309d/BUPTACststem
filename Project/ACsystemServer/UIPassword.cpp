#include "UIPassword.h"
#include "ui_UIPassword.h"

password::password(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::password)
{
    ui->setupUi(this);
}

password::~password()
{
    delete ui;
}

void password::on_ok_pwd_accepted()
{
    emit post_password(ui->text_pwd->text());
}

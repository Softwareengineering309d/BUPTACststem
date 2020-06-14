/*************************
 * 密码UI类
 *  用于输入密码
*************************/
#ifndef UIPASSWORD_H
#define UIPASSWORD_H

#include <QDialog>

namespace Ui {
class password;
}

class password : public QDialog
{
    Q_OBJECT

public:
    explicit password(QWidget *parent = nullptr);
    ~password();

signals:
    void post_password(QString input_pwd);

private slots:

    void on_ok_pwd_accepted();

private:
    Ui::password *ui;

};

#endif // UIPASSWORD_H

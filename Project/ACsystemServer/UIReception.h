/*************************************************
 * ǰ̨UI��
 *  ��ѡ�����ѡ���������˵�/�굥�ķ���
 *  ���"�����˵�"��"�����굥"�����ɺ����Ҳ�Ԥ���޸�
 *  ���"��ӡ"�������ļ������"���"�����Ԥ������
 *
 *  ��Ҫ�ṩ�˵�/�굥��Ϣ�ĺ������Լ���ӡ�˵�/�굥�ĺ���
**************************************************/

#ifndef UIRECEPTION_H
#define UIRECEPTION_H

#include <QMainWindow>

namespace Ui {
class Reception;
}

class Reception : public QMainWindow
{
    Q_OBJECT

public:
    explicit Reception(QWidget *parent = nullptr);
    ~Reception();


private slots:
    void on_invoiceButton_clicked();

    void on_RDRButton_clicked();

    void on_printButton_clicked();

private:
    Ui::Reception *ui;
};

#endif // UIRECEPTION_H

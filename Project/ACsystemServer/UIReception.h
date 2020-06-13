/*************************************************
 * 前台UI类
 *  在选择框内选择需生成账单/详单的房间
 *  点击"生成账单"或"生成详单"，生成后在右侧预览修改
 *  点击"打印"可下载文件，点击"清空"可清空预览内容
 *
 *  需要提供账单/详单信息的函数，以及打印账单/详单的函数
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

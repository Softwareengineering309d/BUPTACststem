#include "UIReception.h"
#include "ui_UIReception.h"
#include "global.h"
#include "QTableWidget"
#include "QFileDialog"
#include <QtPrintSupport/qprinter.h>
#include <QtPrintSupport/qprintdialog.h>

Reception::Reception(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Reception)
{
    ui->setupUi(this);
    for(int i = 1; i <= NUM_ROOM; ++i){
        ui->roomBox->addItem(QString::number(i));
    }
}

Reception::~Reception()
{
    delete ui;
}

// 生成账单
void Reception::on_invoiceButton_clicked()
{
    ui->infoTable->clear();
    ui->infoTable->setRowCount(0);

    // 下拉框中的值为RoomID
    int roomID = ui->roomBox->currentText().toInt();

    //////////////////此处需要数据
    /// 传入json格式或传入QVector<QString>（建议后者），内容如下：
    ///     json:{"打印时间":"20.6.1 12:00","房间号":"1","总费用":"10"}
    ///     QVector:["打印时间","12:00","房间号","1","总费用","10"]
    ///     其中QString需要转为UTF-8格式，QString::fromLocal8Bit("中文")
    // data = get_invoice_data(roomID);


    ///// 为演示效果，此处直接嵌入一些数据，实际上直接循环读入QVector即可
    // 账单只需2列,名称及内容
    ui->infoTable->setColumnCount(2);
    // 打印时间
    QTableWidgetItem *item = new QTableWidgetItem(QString::fromLocal8Bit("打印时间"));
    int row = ui->infoTable->rowCount();
    ui->infoTable->insertRow(row);
    ui->infoTable->setItem(row,0,item);
    item = new QTableWidgetItem("222");
    ui->infoTable->setItem(row,1,item);
    row++;
    // 房间号
    item = new QTableWidgetItem(QString::fromLocal8Bit("房间号"));
    ui->infoTable->insertRow(row);
    ui->infoTable->setItem(row,0,item);
    item = new QTableWidgetItem(QString::number(roomID));
    ui->infoTable->setItem(row,1,item);
    row++;
    // 总费用
    item = new QTableWidgetItem(QString::fromLocal8Bit("总费用"));
    ui->infoTable->insertRow(row);
    ui->infoTable->setItem(row,0,item);
    item = new QTableWidgetItem("234");
    ui->infoTable->setItem(row,1,item);
}

// 生成详单
void Reception::on_RDRButton_clicked()
{
    ui->infoTable->clear();
    ui->infoTable->setRowCount(0);
    // 下拉框中的值为RoomID
    int roomID = ui->roomBox->currentText().toInt();

    //////////////////此处需要数据
    /// 传入json格式或传入QVector<QString>(建议后者)，内容如下：
    ///     json:{"打印时间":"20.6.1 12:00","房间号":"1","总花费":"20",
    ///         "详单1":{"开始时间":"12:10","结束时间":"12:20","风速":"高","花费":"2.5"},
    ///         "详单2":{"开始时间":"12:30","结束时间":"12:40","风速":"低","花费":"3.0"}}
    ///     QVector:["打印时间","12:00","","",
    ///         "房间号","1","","",
    ///         "总费用","20","","",
    ///         "","","","",
    ///         "开始时间","结束时间","风速","花费",
    ///         "12:10","12:20":"高","2.5",
    ///         "12:30","12:40","低","3.0"]
    ///     其中QString需要转为UTF-8格式，QString::fromLocal8Bit("中文")
    // data = get_invoice_data(roomID);

    ///// 为演示效果，此处直接嵌入一些数据，实际上直接循环读入QVector即可
    // 账单需4列
    ui->infoTable->setColumnCount(4);
    // 打印时间
    QTableWidgetItem *item = new QTableWidgetItem(QString::fromLocal8Bit("打印时间"));
    int row = ui->infoTable->rowCount();
    ui->infoTable->insertRow(row);
    ui->infoTable->setItem(row,0,item);
    item = new QTableWidgetItem(QString::fromLocal8Bit("222"));
    ui->infoTable->setItem(row,1,item);
    row++;
    // 房间号
    item = new QTableWidgetItem(QString::fromLocal8Bit("房间号"));
    ui->infoTable->insertRow(row);
    ui->infoTable->setItem(row,0,item);
    item = new QTableWidgetItem(QString::number(roomID));
    ui->infoTable->setItem(row,1,item);
    row++;
    // 总费用
    item = new QTableWidgetItem(QString::fromLocal8Bit("总费用"));
    ui->infoTable->insertRow(row);
    ui->infoTable->setItem(row,0,item);
    item = new QTableWidgetItem("234");
    ui->infoTable->setItem(row,1,item);
    row++;
    // 插入空行
    ui->infoTable->insertRow(row++);
    // 列名
    item = new QTableWidgetItem(QString::fromLocal8Bit("开始时间"));
    ui->infoTable->insertRow(row);
    ui->infoTable->setItem(row,0,item);
    item = new QTableWidgetItem(QString::fromLocal8Bit("结束时间"));
    ui->infoTable->setItem(row,1,item);
    item = new QTableWidgetItem(QString::fromLocal8Bit("风速"));
    ui->infoTable->setItem(row,2,item);
    item = new QTableWidgetItem(QString::fromLocal8Bit("花费"));
    ui->infoTable->setItem(row,3,item);
    row++;
    // 详单
    item = new QTableWidgetItem("12:00");
    ui->infoTable->insertRow(row);
    ui->infoTable->setItem(row,0,item);
    item = new QTableWidgetItem("12:30");
    ui->infoTable->setItem(row,1,item);
    item = new QTableWidgetItem(QString::fromLocal8Bit("高"));
    ui->infoTable->setItem(row,2,item);
    item = new QTableWidgetItem("12");
    ui->infoTable->setItem(row,3,item);
    row++;
}

void Reception::on_printButton_clicked()
{
    QString srcDirPath = QFileDialog::getExistingDirectory(
                   this, "请选择需要打印的文件夹路径","/");
    QString fileName = "test.pdf";

    // 设置输出为高分辨率
    QPrinter printer;
    // 指定输出格式为pdf
    printer.setOutputFormat(QPrinter::PdfFormat);

    printer.setOutputFileName(srcDirPath + "/" + fileName);
    // ui->textEdit->print(&printer);

    ui->infoTable->render(&printer);

}

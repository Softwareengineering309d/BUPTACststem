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

// �����˵�
void Reception::on_invoiceButton_clicked()
{
    ui->infoTable->clear();
    ui->infoTable->setRowCount(0);

    // �������е�ֵΪRoomID
    int roomID = ui->roomBox->currentText().toInt();

    //////////////////�˴���Ҫ����
    /// ����json��ʽ����QVector<QString>��������ߣ����������£�
    ///     json:{"��ӡʱ��":"20.6.1 12:00","�����":"1","�ܷ���":"10"}
    ///     QVector:["��ӡʱ��","12:00","�����","1","�ܷ���","10"]
    ///     ����QString��ҪתΪUTF-8��ʽ��QString::fromLocal8Bit("����")
    // data = get_invoice_data(roomID);


    ///// Ϊ��ʾЧ�����˴�ֱ��Ƕ��һЩ���ݣ�ʵ����ֱ��ѭ������QVector����
    // �˵�ֻ��2��,���Ƽ�����
    ui->infoTable->setColumnCount(2);
    // ��ӡʱ��
    QTableWidgetItem *item = new QTableWidgetItem(QString::fromLocal8Bit("��ӡʱ��"));
    int row = ui->infoTable->rowCount();
    ui->infoTable->insertRow(row);
    ui->infoTable->setItem(row,0,item);
    item = new QTableWidgetItem("222");
    ui->infoTable->setItem(row,1,item);
    row++;
    // �����
    item = new QTableWidgetItem(QString::fromLocal8Bit("�����"));
    ui->infoTable->insertRow(row);
    ui->infoTable->setItem(row,0,item);
    item = new QTableWidgetItem(QString::number(roomID));
    ui->infoTable->setItem(row,1,item);
    row++;
    // �ܷ���
    item = new QTableWidgetItem(QString::fromLocal8Bit("�ܷ���"));
    ui->infoTable->insertRow(row);
    ui->infoTable->setItem(row,0,item);
    item = new QTableWidgetItem("234");
    ui->infoTable->setItem(row,1,item);
}

// �����굥
void Reception::on_RDRButton_clicked()
{
    ui->infoTable->clear();
    ui->infoTable->setRowCount(0);
    // �������е�ֵΪRoomID
    int roomID = ui->roomBox->currentText().toInt();

    //////////////////�˴���Ҫ����
    /// ����json��ʽ����QVector<QString>(�������)���������£�
    ///     json:{"��ӡʱ��":"20.6.1 12:00","�����":"1","�ܻ���":"20",
    ///         "�굥1":{"��ʼʱ��":"12:10","����ʱ��":"12:20","����":"��","����":"2.5"},
    ///         "�굥2":{"��ʼʱ��":"12:30","����ʱ��":"12:40","����":"��","����":"3.0"}}
    ///     QVector:["��ӡʱ��","12:00","","",
    ///         "�����","1","","",
    ///         "�ܷ���","20","","",
    ///         "","","","",
    ///         "��ʼʱ��","����ʱ��","����","����",
    ///         "12:10","12:20":"��","2.5",
    ///         "12:30","12:40","��","3.0"]
    ///     ����QString��ҪתΪUTF-8��ʽ��QString::fromLocal8Bit("����")
    // data = get_invoice_data(roomID);

    ///// Ϊ��ʾЧ�����˴�ֱ��Ƕ��һЩ���ݣ�ʵ����ֱ��ѭ������QVector����
    // �˵���4��
    ui->infoTable->setColumnCount(4);
    // ��ӡʱ��
    QTableWidgetItem *item = new QTableWidgetItem(QString::fromLocal8Bit("��ӡʱ��"));
    int row = ui->infoTable->rowCount();
    ui->infoTable->insertRow(row);
    ui->infoTable->setItem(row,0,item);
    item = new QTableWidgetItem(QString::fromLocal8Bit("222"));
    ui->infoTable->setItem(row,1,item);
    row++;
    // �����
    item = new QTableWidgetItem(QString::fromLocal8Bit("�����"));
    ui->infoTable->insertRow(row);
    ui->infoTable->setItem(row,0,item);
    item = new QTableWidgetItem(QString::number(roomID));
    ui->infoTable->setItem(row,1,item);
    row++;
    // �ܷ���
    item = new QTableWidgetItem(QString::fromLocal8Bit("�ܷ���"));
    ui->infoTable->insertRow(row);
    ui->infoTable->setItem(row,0,item);
    item = new QTableWidgetItem("234");
    ui->infoTable->setItem(row,1,item);
    row++;
    // �������
    ui->infoTable->insertRow(row++);
    // ����
    item = new QTableWidgetItem(QString::fromLocal8Bit("��ʼʱ��"));
    ui->infoTable->insertRow(row);
    ui->infoTable->setItem(row,0,item);
    item = new QTableWidgetItem(QString::fromLocal8Bit("����ʱ��"));
    ui->infoTable->setItem(row,1,item);
    item = new QTableWidgetItem(QString::fromLocal8Bit("����"));
    ui->infoTable->setItem(row,2,item);
    item = new QTableWidgetItem(QString::fromLocal8Bit("����"));
    ui->infoTable->setItem(row,3,item);
    row++;
    // �굥
    item = new QTableWidgetItem("12:00");
    ui->infoTable->insertRow(row);
    ui->infoTable->setItem(row,0,item);
    item = new QTableWidgetItem("12:30");
    ui->infoTable->setItem(row,1,item);
    item = new QTableWidgetItem(QString::fromLocal8Bit("��"));
    ui->infoTable->setItem(row,2,item);
    item = new QTableWidgetItem("12");
    ui->infoTable->setItem(row,3,item);
    row++;
}

void Reception::on_printButton_clicked()
{
    QString srcDirPath = QFileDialog::getExistingDirectory(
                   this, "��ѡ����Ҫ��ӡ���ļ���·��","/");
    QString fileName = "test.pdf";

    // �������Ϊ�߷ֱ���
    QPrinter printer;
    // ָ�������ʽΪpdf
    printer.setOutputFormat(QPrinter::PdfFormat);

    printer.setOutputFileName(srcDirPath + "/" + fileName);
    // ui->textEdit->print(&printer);

    ui->infoTable->render(&printer);

}

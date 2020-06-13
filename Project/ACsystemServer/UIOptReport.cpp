#include "UIOptReport.h"
#include "ui_UIOptReport.h"

OptReport::OptReport(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OptReport)
{
    ui->setupUi(this);
}

OptReport::~OptReport()
{
    delete ui;
}

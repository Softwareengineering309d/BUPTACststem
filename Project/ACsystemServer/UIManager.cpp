#include "UIManager.h"
#include "ui_UIManager.h"
#include "UIOptReport.h"
#include "Report.h"
#include "global.h"


Manager::Manager(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Manager)
{
    ui->setupUi(this);

}

Manager::~Manager()
{
    delete ui;
}


void Manager::on_createButton_clicked()
{
    OptReport* OR = new OptReport;
    OR->exec();
    create_new_report();
}

int Manager::create_new_report() {
    // ·µ»ØrepoetID
    return 0;
}
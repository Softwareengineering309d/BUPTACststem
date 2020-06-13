#ifndef UIOPTREPORT_H
#define UIOPTREPORT_H

#include <QDialog>

namespace Ui {
class OptReport;
}

class OptReport : public QDialog
{
    Q_OBJECT

public:
    explicit OptReport(QWidget *parent = nullptr);
    ~OptReport();

private:
    Ui::OptReport *ui;
};

#endif // UIOPTREPORT_H

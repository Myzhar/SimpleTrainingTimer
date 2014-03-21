#ifndef QINFORMATIONDLG_H
#define QINFORMATIONDLG_H

#include <QDialog>

namespace Ui {
class QInformationDlg;
}

class QInformationDlg : public QDialog
{
    Q_OBJECT

public:
    explicit QInformationDlg(QWidget *parent = 0);
    ~QInformationDlg();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::QInformationDlg *ui;
};

#endif // QINFORMATIONDLG_H

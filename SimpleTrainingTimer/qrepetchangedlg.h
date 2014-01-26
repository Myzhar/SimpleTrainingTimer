#ifndef QREPETCHANGEDLG_H
#define QREPETCHANGEDLG_H

#include <QDialog>

namespace Ui {
class QRepetChangeDlg;
}

class QRepetChangeDlg : public QDialog
{
    Q_OBJECT

public:
    explicit QRepetChangeDlg(int nRepetitions, int nCycles, QWidget *parent = 0);
    ~QRepetChangeDlg();

    void getValues( int& nRepetitions, int& nCycles ){nRepetitions=mRepetitions;nCycles=mCycles;}

protected:
    void changeEvent(QEvent *e);

private slots:
    void on_pushButton_rep_plus_clicked();
    void on_pushButton_rep_minus_clicked();
    void on_pushButton_cyc_plus_clicked();
    void on_pushButton_cyc_minus_clicked();

private:
    Ui::QRepetChangeDlg *ui;

    int mRepetitions;
    int mCycles;
};

#endif // QREPETCHANGEDLG_H

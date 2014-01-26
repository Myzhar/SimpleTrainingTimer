#ifndef QTIMECHANGEDLG_H
#define QTIMECHANGEDLG_H

#include <QDialog>

namespace Ui {
class QTimeChangeDlg;
}

class QTimeChangeDlg : public QDialog
{
    Q_OBJECT

public:
    explicit QTimeChangeDlg(int totSec, QWidget *parent = 0);
    ~QTimeChangeDlg();

    int getTotSeconds();

private:
    void sec2time( int sec );

protected:
    void changeEvent(QEvent *e);

private slots:

    void on_pushButton_hh_plus_clicked();

    void on_pushButton_hh_minus_clicked();

    void on_pushButton_mm_plus_clicked();

    void on_pushButton_mm_minus_clicked();

    void on_pushButton_ss_plus_clicked();

    void on_pushButton_ss_minus_clicked();

private:
    Ui::QTimeChangeDlg *ui;

    int mHours;
    int mMinutes;
    int mSeconds;
};

#endif // QTIMECHANGEDLG_H

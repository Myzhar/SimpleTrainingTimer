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

private:
    Ui::QTimeChangeDlg *ui;

    int mHours;
    int mMinutes;
    int mSeconds;
};

#endif // QTIMECHANGEDLG_H

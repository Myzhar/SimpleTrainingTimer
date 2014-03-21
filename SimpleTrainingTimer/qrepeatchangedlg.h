#ifndef QREPETCHANGEDLG_H
#define QREPETCHANGEDLG_H

#include <QDialog>

namespace Ui {
class QRepetChangeDlg;
}

class QRepeatChangeDlg : public QDialog
{
    Q_OBJECT

public:
    explicit QRepeatChangeDlg(QString title, int nRepetitions, int nCycles, QWidget *parent = 0);
    ~QRepeatChangeDlg();

    void getValues( int& nRepetitions, int& nCycles ){nRepetitions=mRepetitions;nCycles=mCycles;}

protected:
    void changeEvent(QEvent *e);
    void resizeEvent(QResizeEvent *ev) Q_DECL_OVERRIDE;

private slots:
    void on_pushButton_rep_plus_clicked();
    void on_pushButton_rep_minus_clicked();
    void on_pushButton_cyc_plus_clicked();
    void on_pushButton_cyc_minus_clicked();

private:
    Ui::QRepetChangeDlg *ui;

    int mRepetitions;
    int mCycles;

    QString mTitle;
};

#endif // QREPETCHANGEDLG_H

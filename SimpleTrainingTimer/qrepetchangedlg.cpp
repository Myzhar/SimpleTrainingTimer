#include "qrepetchangedlg.h"
#include "ui_qrepetchangedlg.h"

QRepetChangeDlg::QRepetChangeDlg(int nRepetitions, int nCycles, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QRepetChangeDlg)
{
    ui->setupUi(this);

    /*connect( ui->pushButton_ok_2, SIGNAL(clicked()),
             this, SLOT(accept()));
    connect( ui->pushButton_cancel_2, SIGNAL(clicked()),
             this, SLOT(reject()));*/

    mRepetitions = nRepetitions;
    mCycles = nCycles;

    ui->lcdNumber_repetitions->display( mRepetitions );
    ui->lcdNumber_cycles->display( mCycles);
}

QRepetChangeDlg::~QRepetChangeDlg()
{
    delete ui;
}

void QRepetChangeDlg::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type())
    {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void QRepetChangeDlg::on_pushButton_rep_plus_clicked()
{
    mRepetitions++;

    mRepetitions = mRepetitions>99?0:mRepetitions;

    ui->lcdNumber_repetitions->display( mRepetitions );
}

void QRepetChangeDlg::on_pushButton_rep_minus_clicked()
{
    mRepetitions--;

    mRepetitions = mRepetitions<0?99:mRepetitions;

    ui->lcdNumber_repetitions->display( mRepetitions );
}

void QRepetChangeDlg::on_pushButton_cyc_plus_clicked()
{
    mCycles++;

    mCycles = mCycles>99?0:mCycles;

    ui->lcdNumber_cycles->display( mCycles);
}

void QRepetChangeDlg::on_pushButton_cyc_minus_clicked()
{
    mCycles--;

    mCycles = mCycles<0?99:mCycles;

    ui->lcdNumber_cycles->display( mCycles);
}

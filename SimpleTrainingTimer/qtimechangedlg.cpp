#include "qtimechangedlg.h"
#include "ui_qtimechangedlg.h"

QTimeChangeDlg::QTimeChangeDlg(int totSec, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QTimeChangeDlg)
{
    ui->setupUi(this);

    sec2time( totSec );

    ui->lcdNumber_hours->display( mHours );
    ui->lcdNumber_minutes->display( mMinutes );
    ui->lcdNumber_seconds->display( mSeconds );

    /*connect( ui->pushButton_ok, SIGNAL(clicked()),
             this, SLOT(accept()));
    connect( ui->pushButton_cancel, SIGNAL(clicked()),
             this, SLOT(reject()));*/
}

QTimeChangeDlg::~QTimeChangeDlg()
{
    delete ui;
}

void QTimeChangeDlg::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

int QTimeChangeDlg::getTotSeconds()
{
    int sec = mHours*3600+mMinutes*60+mSeconds;

    return sec;
}

void QTimeChangeDlg::sec2time( int sec )
{
    mHours = (sec / 60 / 60) % 24;
    mMinutes = (sec / 60) % 60;
    mSeconds = sec % 60;
}

void QTimeChangeDlg::on_pushButton_hh_plus_clicked()
{
    mHours++;
    mHours=(mHours%60);

    ui->lcdNumber_hours->display(mHours);
}

void QTimeChangeDlg::on_pushButton_hh_minus_clicked()
{
    mHours--;
    mHours = mHours==-1?59:mHours;

    ui->lcdNumber_hours->display(mHours);
}


void QTimeChangeDlg::on_pushButton_mm_plus_clicked()
{
    mMinutes++;
    mMinutes=(mMinutes%60);

    ui->lcdNumber_minutes->display(mMinutes);
}

void QTimeChangeDlg::on_pushButton_mm_minus_clicked()
{
    mMinutes--;
    mMinutes = mMinutes==-1?59:mMinutes;

    ui->lcdNumber_minutes->display(mMinutes);
}

void QTimeChangeDlg::on_pushButton_ss_plus_clicked()
{
    mSeconds++;
    mSeconds=(mSeconds%60);

    ui->lcdNumber_seconds->display(mSeconds);
}

void QTimeChangeDlg::on_pushButton_ss_minus_clicked()
{
    mSeconds--;
    mSeconds = mSeconds==-1?59:mSeconds;

    ui->lcdNumber_seconds->display(mSeconds);
}

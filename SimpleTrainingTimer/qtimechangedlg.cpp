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

    connect( ui->pushButton_ok, SIGNAL(clicked()),
             this, SLOT(accept()));
    connect( ui->pushButton_cancel, SIGNAL(clicked()),
             this, SLOT(reject()));

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




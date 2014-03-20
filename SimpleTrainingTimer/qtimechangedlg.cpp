#include "qtimechangedlg.h"
#include "ui_qtimechangedlg.h"
#include "qscreentools.h"

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

void QTimeChangeDlg::resizeEvent(QResizeEvent *ev)
{
    QDialog::resizeEvent( ev );

    QScreenTools screen;

    int fontPx = screen.cvtMm2Px( 3 );
    QFont font = this->font();
    font.setPixelSize( fontPx );
    ui->label_hours->setFont(font);
    ui->label_minutes->setFont(font);
    ui->label_seconds->setFont(font);

    fontPx = screen.cvtMm2Px( 8 );
    font.setPixelSize( fontPx );
    ui->pushButton_cancel->setFont( font );
    ui->pushButton_hh_minus->setFont( font );
    ui->pushButton_hh_plus->setFont( font );
    ui->pushButton_mm_minus->setFont( font );
    ui->pushButton_mm_plus->setFont( font );
    ui->pushButton_ok->setFont( font );
    ui->pushButton_ss_minus->setFont( font );
    ui->pushButton_ss_plus->setFont( font );

    fontPx = screen.cvtMm2Px( 5 );
    font.setPixelSize( fontPx );
    ui->label->setFont(font);

    fontPx = screen.cvtMm2Px( 15 );
    font.setPixelSize( fontPx );
    ui->label_time->setFont( font );

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

void QTimeChangeDlg::updateTimeStr()
{
    ui->label_time->setText( tr("%1:%2:%3")
                             .arg(mHours,2,10,QChar('0'))
                             .arg(mMinutes,2,10,QChar('0'))
                             .arg(mSeconds,2,10,QChar('0')));
}

void QTimeChangeDlg::on_pushButton_hh_plus_clicked()
{
    mHours++;
    mHours=(mHours%60);

    ui->lcdNumber_hours->display(mHours);
    updateTimeStr();
}

void QTimeChangeDlg::on_pushButton_hh_minus_clicked()
{
    mHours--;
    mHours = mHours==-1?59:mHours;

    ui->lcdNumber_hours->display(mHours);
    updateTimeStr();
}


void QTimeChangeDlg::on_pushButton_mm_plus_clicked()
{
    mMinutes++;
    mMinutes=(mMinutes%60);

    ui->lcdNumber_minutes->display(mMinutes);
    updateTimeStr();
}

void QTimeChangeDlg::on_pushButton_mm_minus_clicked()
{
    mMinutes--;
    mMinutes = mMinutes==-1?59:mMinutes;

    ui->lcdNumber_minutes->display(mMinutes);
    updateTimeStr();
}

void QTimeChangeDlg::on_pushButton_ss_plus_clicked()
{
    mSeconds++;
    mSeconds=(mSeconds%60);

    ui->lcdNumber_seconds->display(mSeconds);
    updateTimeStr();
}

void QTimeChangeDlg::on_pushButton_ss_minus_clicked()
{
    mSeconds--;
    mSeconds = mSeconds==-1?59:mSeconds;

    ui->lcdNumber_seconds->display(mSeconds);
    updateTimeStr();
}

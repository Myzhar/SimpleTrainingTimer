#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // >>>>> LCD Indicators
    ui->lcdNumber_round->setDigitCount( 8 );
    ui->lcdNumber_round->display("00:00:00");

    ui->lcdNumber_pause->setDigitCount( 8 );
    ui->lcdNumber_pause->display("00:00:00");

    ui->lcdNumber_relax->setDigitCount( 8 );
    ui->lcdNumber_relax->display("00:00:00");
    // <<<<< LCD Indicators

    // Initial status
    mStatus = round;

    // >>>>> Timers
    mUpdateTimer.setTimerType( Qt::PreciseTimer );
    mUpdateTimer.setInterval( 1000 );

    mUpdateTimer.setTimerType( Qt::PreciseTimer );

    connect( &mUpdateTimer, SIGNAL(timeout()),
             this, SLOT(onUpdateTimerTimeout()) );
    // <<<<< Timers

    mRoundDuration = 5;
    mPauseDuration = 10;
    mRelaxDuration = 15;

    mRepetition = 2;
    mCycles = 2;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void MainWindow::on_pushButton_start_pause_clicked()
{
    if( mUpdateTimer.isActive() ) // Pause
    {
        mUpdateTimer.stop();
        ui->pushButton_start_pause->setText( tr("Start") );
    }
    else // Start
    {
        mUpdateTimer.start();
        ui->pushButton_start_pause->setText( tr("Pause") );
    }
}

QString MainWindow::sec2str( quint32 sec )
{
    QTime startTime(0,0,0);
    QTime time;
    QString timeStr;

    time = startTime.addSecs( sec );
    timeStr = time.toString( "HH:mm:ss" );

    return timeStr;
}

void MainWindow::on_pushButton_reset_clicked()
{
    ui->lcdNumber_round->display(sec2str(mRoundDuration));
    ui->lcdNumber_pause->display(sec2str(mPauseDuration));
    ui->lcdNumber_relax->display(sec2str(mRelaxDuration));

    ui->spinBox_repetitions->setValue( mRepetition );
    ui->spinBox_cycles->setValue( mCycles );

    if( mUpdateTimer.isActive() ) // Pause
    {
        mUpdateTimer.stop();
        ui->pushButton_start_pause->setText( tr("Start") );
    }

    mDownTime = mRoundDuration;
    mDownRepetition = mRepetition;
    mDownCycles = mCycles;
}

void MainWindow::onUpdateTimerTimeout()
{
    mDownTime--;

    qDebug() << Q_FUNC_INFO << tr("Update - DownTime val: %d").arg(mDownTime);

    switch(mStatus)
    {
    case round:
    {
        if(mDownTime==0)
        {
            if( mDownCycles==0 ) // Stop
            {
                on_pushButton_reset_clicked();
            }

            if( mDownRepetition==0 )
            {
                mDownCycles--;
                mStatus = relax;
                mDownTime = mRelaxDuration;
                ui->lcdNumber_round->display(sec2str(mRoundDuration));
                ui->lcdNumber_relax->display(sec2str(mRelaxDuration));
            }
            else
            {
                mDownRepetition--;
                mStatus = pause;
                mDownTime = mPauseDuration;
                ui->lcdNumber_round->display(sec2str(mRoundDuration));
                ui->lcdNumber_pause->display(sec2str(mDownTime));
            }
        }
        else
            ui->lcdNumber_round->display(sec2str(mDownTime));

        break;
    }

    case pause:
    {
        if(mDownTime==0)
        {
            mStatus = round;
            mDownTime = mRoundDuration;
            ui->lcdNumber_pause->display(sec2str(mPauseDuration));
            ui->lcdNumber_round->display(sec2str(mDownTime));
        }
        else
            ui->lcdNumber_pause->display(sec2str(mDownTime));

        break;
    }

    case relax:
    {
        if(mDownTime==0)
        {
            mStatus = round;  // GESTIRE I CICLI
            mDownTime = mRoundDuration;
            ui->lcdNumber_relax->display(sec2str(mRelaxDuration));
            ui->lcdNumber_round->display(sec2str(mDownTime));
        }
        else
            ui->lcdNumber_relax->display(sec2str(mDownTime));

        break;
    }
    }
}



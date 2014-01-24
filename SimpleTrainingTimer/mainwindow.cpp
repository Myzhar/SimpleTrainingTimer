#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QSound>
#include <QLCDNumber>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // >>>>> LCD Indicators
    QPalette pal;

    ui->lcdNumber_round->setDigitCount( 8 );
    ui->lcdNumber_round->display("00:00:00");
    ui->lcdNumber_round->setAutoFillBackground(true);
    pal = ui->lcdNumber_round->palette();
    pal.setColor( QPalette::Background, QColor( 180,180,230 ));
    ui->lcdNumber_round->setPalette( pal );

    ui->lcdNumber_pause->setDigitCount( 8 );
    ui->lcdNumber_pause->display("00:00:00");
    ui->lcdNumber_pause->setAutoFillBackground(true);
    pal = ui->lcdNumber_pause->palette();
    pal.setColor( QPalette::Background, QColor( 180,230,180 ));
    ui->lcdNumber_pause->setPalette( pal );

    ui->lcdNumber_relax->setDigitCount( 8 );
    ui->lcdNumber_relax->display("00:00:00");
    ui->lcdNumber_relax->setAutoFillBackground(true);
    pal = ui->lcdNumber_relax->palette();
    pal.setColor( QPalette::Background, QColor( 230,180,180 ));
    ui->lcdNumber_relax->setPalette( pal );

    ui->lcdNumber_elapsed->setDigitCount( 8 );
    ui->lcdNumber_elapsed->display("00:00:00");
    ui->lcdNumber_elapsed->setAutoFillBackground(true);
    pal = ui->lcdNumber_elapsed->palette();
    pal.setColor( QPalette::Background, QColor( 230,230,180 ));
    ui->lcdNumber_elapsed->setPalette( pal );

    ui->lcdNumber_remaining->setDigitCount( 8 );
    ui->lcdNumber_remaining->display("00:00:00");
    ui->lcdNumber_remaining->setAutoFillBackground(true);
    pal = ui->lcdNumber_remaining->palette();
    pal.setColor( QPalette::Background, QColor( 230,180,230 ));
    ui->lcdNumber_remaining->setPalette( pal );
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

    mRoundDuration = 30;
    mPauseDuration = 15;
    mRelaxDuration = 20;

    mSignalTime = 10;

    mRepetition = 3;
    mCycles = 2;

    ResetTimer();
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
        mDownRepetitionResetVal = mDownRepetition;

        QSound::play(":/Sounds/Whistle_high.wav");
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
    ResetTimer();
}

void MainWindow::ResetTimer()
{
    mRemaining = (( (mRoundDuration*mRepetition) + (mPauseDuration*(mRepetition-1)) ) * mCycles) + ( mRelaxDuration*(mCycles-1) );
    mElapsed = 0;

    ui->lcdNumber_round->display(sec2str(mRoundDuration));
    ui->lcdNumber_pause->display(sec2str(mPauseDuration));
    ui->lcdNumber_relax->display(sec2str(mRelaxDuration));

    ui->lcdNumber_repetitions->display( mRepetition );
    ui->lcdNumber_cycles->display( mCycles );

    ui->lcdNumber_elapsed->display(sec2str(0));
    ui->lcdNumber_remaining->display(sec2str(mRemaining));

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
    mElapsed++;
    mRemaining--;

    ui->lcdNumber_elapsed->display(sec2str(mElapsed));
    ui->lcdNumber_remaining->display(sec2str(mRemaining));

    if( mDownTime==mSignalTime )
    {
        QSound::play(":/Sounds/Signal.wav");
    }

    qDebug() << Q_FUNC_INFO << tr("Update - DownTime val: %1").arg(mDownTime);

    switch(mStatus)
    {
    case round:
    {
        if(mDownTime==0)
        {

            if( mDownRepetition==1 )
            {
                if( mDownCycles==1 ) // Stop
                {
                    on_pushButton_reset_clicked();
                    QSound::play(":/Sounds/3x_Whistle.wav");
                }
                else
                {
                    mDownCycles--;
                    ui->lcdNumber_cycles->display(mDownCycles);

                    mStatus = relax;
                    QSound::play(":/Sounds/Bell.wav");

                    mDownTime = mRelaxDuration;
                    ui->lcdNumber_round->display(sec2str(mRoundDuration));
                    ui->lcdNumber_relax->display(sec2str(mRelaxDuration));
                }
            }
            else
            {
                mDownRepetition--;
                ui->lcdNumber_repetitions->display(mDownRepetition);

                mStatus = pause;
                QSound::play(":/Sounds/Whistle.wav");

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
            QSound::play(":/Sounds/Whistle_high.wav");

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
            mStatus = round;
            QSound::play(":/Sounds/Whistle_high.wav");
            mDownRepetition = mDownRepetitionResetVal;

            mDownTime = mRoundDuration;
            ui->lcdNumber_relax->display(sec2str(mRelaxDuration));
            ui->lcdNumber_round->display(sec2str(mDownTime));
            ui->lcdNumber_repetitions->display(mRepetition);
        }
        else
            ui->lcdNumber_relax->display(sec2str(mDownTime));

        break;
    }
    }
}



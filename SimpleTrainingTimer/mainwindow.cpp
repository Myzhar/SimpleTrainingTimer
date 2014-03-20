#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QSound>
#include <QLCDNumber>
#include <QDir>
#include "qtimecomposingwidget.h"
#include "qtimechangedlg.h"
#include "qrepetchangedlg.h"
#include "qscreentools.h"

#define CHANGE_LAB_COLOR QColor( 120,120,210 )
#define FIXED_LAB_COLOR QColor( 200,200,100 )


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    mIniSettings(NULL)
{
    ui->setupUi(this);

    // >>>>> INI
#ifndef ANDROID
    mIniPath = QApplication::applicationDirPath();
    mIniPath += tr("%1.ini").arg(QApplication::applicationName());
#else
    // under Android it is not possible to write in the binary folder. We must store our "ini" file in a new folder under /sdcard/
    mIniPath = tr("/sdcard/%1/").arg(INI_FOLDER);
    QDir dir(mIniPath);
    if( !dir.exists() )
    {
        dir.mkdir(mIniPath);
    }
    mIniPath += tr("%1.ini").arg(INI_FOLDER);
#endif

    mIniSettings = new QSettings( mIniPath, QSettings::IniFormat, this );

    mRoundDuration = mIniSettings->value( "mRoundDuration", 60).toInt();
    mPauseDuration = mIniSettings->value( "mPauseDuration", 30).toInt();
    mRelaxDuration = mIniSettings->value( "mRelaxDuration", 120).toInt();
    mRepetitions = mIniSettings->value( "mRepetitions", 3).toInt();
    mCycles = mIniSettings->value( "mCycles", 5).toInt();

    updateIni();
    // <<<<< INI

    mSignalConnected=false;
    mPaused=false;

    // >>>>> LCD Indicators
    QPalette pal;

    ui->lcdNumber_round->setDigitCount( 8 );
    ui->lcdNumber_round->display("00:00:00");
    ui->lcdNumber_round->setAutoFillBackground(true);
    pal = ui->lcdNumber_round->palette();
    pal.setColor( QPalette::Background, ROUND_COLOR );
    ui->lcdNumber_round->setPalette( pal );

    ui->lcdNumber_pause->setDigitCount( 8 );
    ui->lcdNumber_pause->display("00:00:00");
    ui->lcdNumber_pause->setAutoFillBackground(true);
    pal = ui->lcdNumber_pause->palette();
    pal.setColor( QPalette::Background,PAUSE_COLOR );
    ui->lcdNumber_pause->setPalette( pal );

    ui->lcdNumber_relax->setDigitCount( 8 );
    ui->lcdNumber_relax->display("00:00:00");
    ui->lcdNumber_relax->setAutoFillBackground(true);
    pal = ui->lcdNumber_relax->palette();
    pal.setColor( QPalette::Background, RELAX_COLOR );
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

    ui->label_cycles->setAutoFillBackground(true);
    pal = ui->label_cycles->palette();
    pal.setColor( QPalette::Background, CHANGE_LAB_COLOR );
    ui->label_cycles->setPalette( pal );

    ui->label_pause->setAutoFillBackground(true);
    pal = ui->label_pause->palette();
    pal.setColor( QPalette::Background, CHANGE_LAB_COLOR );
    ui->label_pause->setPalette( pal );

    ui->label_relax->setAutoFillBackground(true);
    pal = ui->label_relax->palette();
    pal.setColor( QPalette::Background, CHANGE_LAB_COLOR );
    ui->label_relax->setPalette( pal );

    ui->label_repetitions->setAutoFillBackground(true);
    pal = ui->label_repetitions->palette();
    pal.setColor( QPalette::Background, CHANGE_LAB_COLOR );
    ui->label_repetitions->setPalette( pal );

    ui->label_round->setAutoFillBackground(true);
    pal = ui->label_round->palette();
    pal.setColor( QPalette::Background, CHANGE_LAB_COLOR );
    ui->label_round->setPalette( pal );

    ui->label_elapsed->setAutoFillBackground(true);
    pal = ui->label_elapsed->palette();
    pal.setColor( QPalette::Background, FIXED_LAB_COLOR );
    ui->label_elapsed->setPalette( pal );

    ui->label_remaining->setAutoFillBackground(true);
    pal = ui->label_remaining->palette();
    pal.setColor( QPalette::Background, FIXED_LAB_COLOR );
    ui->label_remaining->setPalette( pal );
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


    mSignalTime = 10;

    updateGui();

    // >>>>> Time indicators


    ui->widget_time_indicator_dx->setTimePosition( 0 );
    ui->widget_time_indicator_sx->setTimePosition( 0 );
    // <<<<< Time indicators

    connectSignals( true );

    ResetTimer();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::resizeEvent(QResizeEvent * ev)
{
    QMainWindow::resizeEvent( ev );

    QScreenTools screen;

    int fontPx = screen.cvtMm2Px( 3 );
    QFont font = this->font();
    font.setPixelSize( fontPx );
    this->setFont( font );
    ui->label_cycles->setFont( font );
    ui->label_elapsed->setFont( font );
    ui->label_pause->setFont( font );
    ui->label_relax->setFont( font );
    ui->label_remaining->setFont( font );
    ui->label_repetitions->setFont( font );
    ui->label_round->setFont( font );

    fontPx = screen.cvtMm2Px( 8 );
    font.setPixelSize( fontPx);
    ui->pushButton_start_pause->setFont( font );
    ui->pushButton_reset->setFont( font );
    ui->pushButton_start_pause->setIconSize( QSize( fontPx,fontPx ));
    ui->pushButton_reset->setIconSize( QSize( fontPx,fontPx));


    ui->widget_time_indicator_dx->setMinimumWidth( this->width()/6 );
    ui->widget_time_indicator_sx->setMinimumWidth( this->width()/6 );
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
        ui->pushButton_start_pause->setText( tr("Resume") );
        ui->pushButton_start_pause->setIcon( QIcon( "://Images/Actions-player-play-icon.png" ) );

        mPaused = true;

        //connectSignals( true );
    }
    else
    {
        if(!mPaused)// Start
        {
            mPaused = false;
            mRepetTimer=0;
            mUpdateTimer.start();
            ui->pushButton_start_pause->setText( tr("Pause") );
            ui->pushButton_start_pause->setIcon( QIcon( "://Images/Actions-player-pause-icon.png" ) );
            mDownRepetitionResetVal = mDownRepetition;

            QSound::play(":/Sounds/Whistle_high.wav");

            connectSignals( false );
        }
        else
        {
            mPaused = false;
            mUpdateTimer.start();
            ui->pushButton_start_pause->setText( tr("Pause") );
            ui->pushButton_start_pause->setIcon( QIcon( "://Images/Actions-player-pause-icon.png" ) );

            QSound::play(":/Sounds/Whistle_high.wav");
        }
    }
}

QString MainWindow::sec2str( int sec )
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
    mRemaining = (( (mRoundDuration*mRepetitions) + (mPauseDuration*(mRepetitions-1)) ) * mCycles) + ( mRelaxDuration*(mCycles-1) );
    mElapsed = 0;
    mPaused = false;

    ui->lcdNumber_round->display(sec2str(mRoundDuration));
    ui->lcdNumber_pause->display(sec2str(mPauseDuration));
    ui->lcdNumber_relax->display(sec2str(mRelaxDuration));

    ui->lcdNumber_repetitions->display( mRepetitions );
    ui->lcdNumber_cycles->display( mCycles );

    ui->lcdNumber_elapsed->display(sec2str(0));
    ui->lcdNumber_remaining->display(sec2str(mRemaining));

    ui->widget_time_indicator_dx->setTimePosition( mElapsed );
    ui->widget_time_indicator_sx->setTimePosition( mElapsed );

    if( mUpdateTimer.isActive() ) // Pause
    {
        mUpdateTimer.stop();
        ui->pushButton_start_pause->setText( tr("Start") );
        ui->pushButton_start_pause->setIcon( QIcon( "://Images/Actions-player-play-icon.png" ) );
    }

    mDownTime = mRoundDuration;
    mDownRepetition = mRepetitions;
    mDownCycles = mCycles;

    connectSignals( true );
}

void MainWindow::connectSignals( bool connectState )
{
    if( connectState && !mSignalConnected )
    {
        connect(ui->lcdNumber_round, SIGNAL(clicked()),
                this, SLOT(onLcdRoundClicked()) );
        connect(ui->lcdNumber_pause, SIGNAL(clicked()),
                this, SLOT(onLcdPauseClicked()) );
        connect(ui->lcdNumber_relax, SIGNAL(clicked()),
                this, SLOT(onLcdRelaxClicked()) );
        connect(ui->lcdNumber_repetitions, SIGNAL(clicked()),
                this, SLOT(onLcdRepetCycClicked()) );
        connect(ui->lcdNumber_cycles, SIGNAL(clicked()),
                this, SLOT(onLcdRepetCycClicked()) );

        mSignalConnected = true;
    }

    if( !connectState && mSignalConnected )
    {
        disconnect(ui->lcdNumber_round, SIGNAL(clicked()),
                   this, SLOT(onLcdRoundClicked()) );
        disconnect(ui->lcdNumber_pause, SIGNAL(clicked()),
                   this, SLOT(onLcdPauseClicked()) );
        disconnect(ui->lcdNumber_relax, SIGNAL(clicked()),
                   this, SLOT(onLcdRelaxClicked()) );
        disconnect(ui->lcdNumber_repetitions, SIGNAL(clicked()),
                   this, SLOT(onLcdRepetCycClicked()) );
        disconnect(ui->lcdNumber_cycles, SIGNAL(clicked()),
                   this, SLOT(onLcdRepetCycClicked()) );

        mSignalConnected = false;
    }
}

void MainWindow::onUpdateTimerTimeout()
{
    mDownTime--;
    mElapsed++;
    mRemaining--;

    mRepetTimer++;

    ui->lcdNumber_elapsed->display(sec2str(mElapsed));
    ui->lcdNumber_remaining->display(sec2str(mRemaining));

    ui->widget_time_indicator_dx->setTimePosition( mRepetTimer );
    ui->widget_time_indicator_sx->setTimePosition( mRepetTimer );

    if( mDownTime==mSignalTime )
    {
        QSound::play(":/Sounds/Signal.wav");
    }

    //qDebug() << Q_FUNC_INFO << tr("Update - DownTime val: %1").arg(mDownTime);

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
            ui->lcdNumber_repetitions->display(mRepetitions);

            mRepetTimer=0;
        }
        else
            ui->lcdNumber_relax->display(sec2str(mDownTime));

        break;
    }
    }
}

void MainWindow::updateIni()
{
   mIniSettings->setValue( "mRoundDuration", mRoundDuration );
   mIniSettings->setValue( "mPauseDuration", mPauseDuration );
   mIniSettings->setValue( "mRelaxDuration", mRelaxDuration );

   mIniSettings->setValue( "mRepetitions", mRepetitions );
   mIniSettings->setValue( "mCycles", mCycles );

   mIniSettings->sync();
}

void MainWindow::updateGui()
{
    ui->widget_time_indicator_dx->setTimingParams( mRoundDuration,
                                                   mPauseDuration,
                                                   mRelaxDuration,
                                                   mRepetitions,
                                                   mCycles );

    ui->widget_time_indicator_sx->setTimingParams( mRoundDuration,
                                                   mPauseDuration,
                                                   mRelaxDuration,
                                                   mRepetitions,
                                                   mCycles );

    mRemaining = (( (mRoundDuration*mRepetitions) + (mPauseDuration*(mRepetitions-1)) ) * mCycles) + ( mRelaxDuration*(mCycles-1) );
    ui->lcdNumber_remaining->display( sec2str(mRemaining) );
}

void MainWindow::onLcdRoundClicked()
{
    QTimeChangeDlg dlg( mRoundDuration, this );
    dlg.setWindowState(dlg.windowState() | Qt::WindowMaximized);
    int res = dlg.exec();

    if( res == QDialog::Accepted )
    {
        mRoundDuration = dlg.getTotSeconds();
        ui->lcdNumber_round->display( sec2str(mRoundDuration));

        updateGui();
        updateIni();
    }
}

void MainWindow::onLcdPauseClicked()
{
    QTimeChangeDlg dlg( mPauseDuration, this );
    dlg.setWindowState(dlg.windowState() | Qt::WindowMaximized);
    int res = dlg.exec();

    if( res == QDialog::Accepted )
    {
        mPauseDuration = dlg.getTotSeconds();
        ui->lcdNumber_pause->display( sec2str(mPauseDuration));

        updateGui();
        updateIni();
    }
}

void MainWindow::onLcdRelaxClicked()
{
    QTimeChangeDlg dlg( mRelaxDuration, this );
    dlg.setWindowState(dlg.windowState() | Qt::WindowMaximized);
    int res = dlg.exec();

    if( res == QDialog::Accepted )
    {
        mRelaxDuration = dlg.getTotSeconds();
        ui->lcdNumber_relax->display( sec2str(mRelaxDuration));

        updateGui();
        updateIni();
    }
}

void MainWindow::onLcdRepetCycClicked()
{
    QRepetChangeDlg dlg( mRepetitions, mCycles, this );
    dlg.setWindowState(dlg.windowState() | Qt::WindowMaximized);
    int res = dlg.exec();

    if( res == QDialog::Accepted )
    {
        dlg.getValues( mRepetitions, mCycles );

        ui->lcdNumber_repetitions->display( mRepetitions );
        ui->lcdNumber_cycles->display( mCycles );

        updateGui();
        updateIni();
    }
}


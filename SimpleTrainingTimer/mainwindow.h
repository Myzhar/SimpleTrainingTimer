#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QTime>
#include <QMediaPlayer>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

    typedef enum _Status { round, pause, relax } Status;

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    virtual void resizeEvent(QResizeEvent * ev) Q_DECL_OVERRIDE;
    void changeEvent(QEvent *e);

    QString sec2str( int sec );
    void ResetTimer();

    void updateGui();

    void connectSignals(bool connectState );

private slots:
    void on_pushButton_start_pause_clicked();
    void on_pushButton_reset_clicked();

    void onUpdateTimerTimeout();

    void onLcdRoundClicked();
    void onLcdPauseClicked();
    void onLcdRelaxClicked();

    void onLcdRepetCycClicked();

private:
    Ui::MainWindow *ui;

    Status mStatus;

    QTimer mUpdateTimer;

    bool mSignalConnected;
    bool mPaused;

    int mDownTime;
    int mDownRepetition;
    int mDownRepetitionResetVal;
    int mDownCycles;

    int mElapsed;
    int mRemaining;

    int mRepetTimer;

    int mRoundDuration;
    int mPauseDuration;
    int mRelaxDuration;

    int mRepetitions;
    int mCycles;


    int mSignalTime;
};

#endif // MAINWINDOW_H

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QTime>

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
    void changeEvent(QEvent *e);

    QString sec2str( quint32 sec );

private slots:
    void on_pushButton_start_pause_clicked();
    void on_pushButton_reset_clicked();

    void onUpdateTimerTimeout();

private:
    Ui::MainWindow *ui;

    Status mStatus;

    QTimer mUpdateTimer;

    quint32 mDownTime;
    quint16 mDownRepetition;
    quint16 mDownCycles;

    quint32 mRoundDuration;
    quint32 mPauseDuration;
    quint32 mRelaxDuration;

    quint16 mRepetition;
    quint16 mCycles;
};

#endif // MAINWINDOW_H

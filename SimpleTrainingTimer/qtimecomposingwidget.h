#ifndef QTIMECOMPOSINGWIDGET_H
#define QTIMECOMPOSINGWIDGET_H

#include <QWidget>

#define ROUND_COLOR QColor( 180,180,230 )
#define PAUSE_COLOR QColor( 180,230,180 )
#define RELAX_COLOR QColor( 230,80,80 )

class QTimeComposingWidget : public QWidget
{
    Q_OBJECT
public:
    explicit QTimeComposingWidget(QWidget *parent = 0);

protected:
    virtual void resizeEvent(QResizeEvent * event) Q_DECL_OVERRIDE;
    virtual void paintEvent(QPaintEvent* event) Q_DECL_OVERRIDE;

signals:

public slots:
    void setTimingParams(int roundDuration, int pauseDuration,
                          int relaxDuration, int repetition,
                          int cycles );

    void setTimePosition( int currentTime );

private:
    int mCurrTime;

    int mRoundDuration;
    int mPauseDuration;
    int mRelaxDuration;

    int mRepetition;
    int mCycles;

    int mTotTime;
};

#endif // QTIMECOMPOSINGWIDGET_H

#include "qtimecomposingwidget.h"
#include <QPainter>
#include <QStyle>
#include <QStyleOption>

QTimeComposingWidget::QTimeComposingWidget(QWidget *parent) :
    QWidget(parent)
{

}

void QTimeComposingWidget::resizeEvent(QResizeEvent* event)
{
    Q_UNUSED(event);
}

void QTimeComposingWidget::paintEvent( QPaintEvent* event )
{
    Q_UNUSED(event);

    QPainter painter(this);

    painter.setPen(Qt::NoPen);

    QBrush roundBrush( ROUND_COLOR );
    QBrush pauseBrush( PAUSE_COLOR );
    QBrush relaxBrush( RELAX_COLOR );

    double scaleFact = ((double)height())/((double)mTotTime);

    int roundH = (int)((scaleFact*(double)mRoundDuration)/*+0.5*/);
    int pauseH = (int)((scaleFact*(double)mPauseDuration)/*+0.5*/);
    int relaxH = (int)((scaleFact*(double)mRelaxDuration)/*+0.5*/);

    int relaxPos=0;
    //int startPos=0;

    for( int i=0; i<mRepetition; i++ )
    {
        painter.setBrush( roundBrush );
        painter.drawRect( 0, i*(roundH+pauseH), this->width(), roundH );
        relaxPos+=(roundH);
    }

    for( int i=0; i<mRepetition-1; i++ )
    {
        painter.setBrush( pauseBrush );
        painter.drawRect( 0, roundH+i*(roundH+pauseH), this->width(), pauseH );
        relaxPos+=(pauseH);
    }

    painter.setBrush( relaxBrush );
    painter.drawRect( 0, relaxPos, this->width(), relaxH );

    QPen timeIndicatorPen( QColor( 255,100,100) );
    timeIndicatorPen.setWidth(3);

    int timeIndicatorPos = (int)(((double)(this->height()-2)/((double)mTotTime)*(double)mCurrTime)+0.5);

    painter.setPen( timeIndicatorPen );
    painter.setBrush( OVERPAINT_COLOR );
    painter.drawLine( 1, timeIndicatorPos, this->width()-2, timeIndicatorPos );
    painter.setPen( Qt::NoPen );
    painter.drawRect(0,0,this->width()-1, timeIndicatorPos );
}

void QTimeComposingWidget::setTimingParams( int roundDuration, int pauseDuration,
                                            int relaxDuration, int repetition,
                                            int cycles )
{
    mRoundDuration = roundDuration;
    mPauseDuration = pauseDuration;
    mRelaxDuration = relaxDuration;

    mRepetition = repetition;
    mCycles = cycles;

    //mTotTime = (( (mRoundDuration*mRepetition) + (mPauseDuration*(mRepetition-1)) ) * mCycles) + ( mRelaxDuration*(mCycles-1) );
    mTotTime = ( (mRoundDuration*mRepetition) + (mPauseDuration*(mRepetition-1)) ) + mRelaxDuration;

    update();
}

void QTimeComposingWidget::setTimePosition( int currentTime )
{
    mCurrTime = currentTime;

    update();
}

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

    /*int widgetH = height();
    int totH = (( (roundH*mRepetition) + (pauseH*(mRepetition-1)) ) * mCycles) + ( relaxH*(mCycles-1) );*/

    int relaxPos=0;
    int startPos=0;

    /*for( int c=0; c<mCycles; c++ )
    {
        for( int i=0; i<mRepetition; i++ )
        {
            painter.setBrush( roundBrush );
            painter.drawRect( 0, startPos+i*(roundH+pauseH-1), this->width()-1, roundH );
            relaxPos+=(roundH);
        }

        for( int i=0; i<mRepetition-1; i++ )
        {
            painter.setBrush( pauseBrush );
            painter.drawRect( 0, startPos+roundH+i*(roundH+pauseH-1), this->width()-1, pauseH );
            relaxPos+=(pauseH);
        }

        if( c!=mCycles-1) // Last relax interval must not be rendered
        {
            painter.setBrush( relaxBrush );
            painter.drawRect( 0, relaxPos, this->width()-1, relaxH );

            startPos = relaxPos;
            startPos += (relaxH);
            relaxPos = startPos;
        }
    }*/

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
    painter.drawLine( 1, timeIndicatorPos, this->width()-2, timeIndicatorPos );
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
}

void QTimeComposingWidget::setTimePosition( int currentTime )
{
    mCurrTime = currentTime;

    update();
}

#include "qclicklcdnumber.h"
#include <QEvent>

QClickLCDNumber::QClickLCDNumber(QWidget *parent) :
    QLCDNumber(parent)
{
}

void QClickLCDNumber::mouseReleaseEvent( QMouseEvent * event )
{
    Q_UNUSED(event);

    emit clicked();
}

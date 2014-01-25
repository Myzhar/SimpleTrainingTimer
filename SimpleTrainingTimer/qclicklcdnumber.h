#ifndef QCLICKLCDNUMBER_H
#define QCLICKLCDNUMBER_H

#include <QLCDNumber>

class QClickLCDNumber : public QLCDNumber
{
    Q_OBJECT
public:
    explicit QClickLCDNumber(QWidget *parent = 0);

protected:
    virtual void mouseReleaseEvent( QMouseEvent * event );

signals:
    void clicked();

private:


};

#endif // QCLICKLCDNUMBER_H

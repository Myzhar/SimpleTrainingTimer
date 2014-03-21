#include "qrepeatchangedlg.h"
#include "ui_qrepetchangedlg.h"
#include "qscreentools.h"
#include "objectsizes.h"

QRepeatChangeDlg::QRepeatChangeDlg(QString title, int nRepetitions, int nCycles, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QRepetChangeDlg)
{
    ui->setupUi(this);

    /*connect( ui->pushButton_ok_2, SIGNAL(clicked()),
             this, SLOT(accept()));
    connect( ui->pushButton_cancel_2, SIGNAL(clicked()),
             this, SLOT(reject()));*/

    mRepetitions = nRepetitions;
    mCycles = nCycles;

    ui->lcdNumber_repetitions->display( mRepetitions );
    ui->lcdNumber_cycles->display( mCycles);

    mTitle = title;
    ui->label_title->setText(mTitle);
}

QRepeatChangeDlg::~QRepeatChangeDlg()
{
    delete ui;
}

void QRepeatChangeDlg::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type())
    {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void QRepeatChangeDlg::resizeEvent(QResizeEvent *ev)
{
    QDialog::resizeEvent( ev );

    QScreenTools screen;

    int fontPx = screen.cvtMm2Px( LABEL_FIELD_TITLE_MM );
    QFont font = this->font();
    font.setPixelSize( fontPx );
    ui->label_cycles->setFont(font);
    ui->label_repetitions->setFont(font);

    fontPx = screen.cvtMm2Px( LABEL_PUSHBUTTON_MM );
    font.setPixelSize( fontPx );
    ui->pushButton_cancel_2->setFont( font );
    ui->pushButton_cyc_minus->setFont( font );
    ui->pushButton_cyc_plus->setFont( font );
    ui->pushButton_ok_2->setFont( font );
    ui->pushButton_rep_minus->setFont( font );
    ui->pushButton_rep_plus->setFont( font );

    fontPx = screen.cvtMm2Px( DIALOG_TITLE_MM );
    font.setPixelSize( fontPx );
    ui->label_title->setFont( font );
}

void QRepeatChangeDlg::on_pushButton_rep_plus_clicked()
{
    mRepetitions++;

    mRepetitions = mRepetitions>99?0:mRepetitions;

    ui->lcdNumber_repetitions->display( mRepetitions );
}

void QRepeatChangeDlg::on_pushButton_rep_minus_clicked()
{
    mRepetitions--;

    mRepetitions = mRepetitions<0?99:mRepetitions;

    ui->lcdNumber_repetitions->display( mRepetitions );
}

void QRepeatChangeDlg::on_pushButton_cyc_plus_clicked()
{
    mCycles++;

    mCycles = mCycles>99?0:mCycles;

    ui->lcdNumber_cycles->display( mCycles);
}

void QRepeatChangeDlg::on_pushButton_cyc_minus_clicked()
{
    mCycles--;

    mCycles = mCycles<0?99:mCycles;

    ui->lcdNumber_cycles->display( mCycles);
}

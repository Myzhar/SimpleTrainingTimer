#include "qinformationdlg.h"
#include "ui_qinformationdlg.h"
#include "objectsizes.h"
#include "qscreentools.h"

QInformationDlg::QInformationDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QInformationDlg)
{
    ui->setupUi(this);

    QFont font = this->font();

    QScreenTools screen;

    int size = screen.cvtMm2Px( LABEL_PUSHBUTTON_MM );
    font.setPixelSize( size );
    ui->buttonBox->setFont( font );

    size = screen.cvtMm2Px( DIALOG_TITLE_MM );
    font.setPixelSize( size );
    ui->label->setFont( font );
}

QInformationDlg::~QInformationDlg()
{
    delete ui;
}

void QInformationDlg::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

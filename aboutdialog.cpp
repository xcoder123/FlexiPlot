#include "aboutdialog.h"
#include "ui_aboutdialog.h"

AboutDialog::AboutDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutDialog)
{
    ui->setupUi(this);


    connect(ui->closeBtn, SIGNAL(clicked(bool)), this, SLOT(close()));


    ui->basedLbl->setText(QString("Based on Qt %1 ( Build Arch: %2)").arg(QT_VERSION_STR).arg(QSysInfo::buildCpuArchitecture()) );
    ui->builtLbl->setText("Built on "  + QString::fromLocal8Bit( __DATE__ ) + " " + QString::fromLocal8Bit(__TIME__));
}

AboutDialog::~AboutDialog()
{
    delete ui;
}

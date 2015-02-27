#include "settings.h"
#include "ui_settings.h"

Settings::Settings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Settings)
{
    ui->setupUi(this);

    QList<QSerialPortInfo> ports = QSerialPortInfo::availablePorts();

    foreach(QSerialPortInfo port, ports)
        ui->serialPort->addItem(port.portName());

    foreach(qint32 baudrate, QSerialPortInfo::standardBaudRates())
        ui->baudrate->addItem(QString::number( baudrate ));

    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(saveSettings()));    
    loadSettings();
}


//SAves settings in the registry
void Settings::saveSettings()
{
    QSettings settings("HKEY_CURRENT_USER\\Software\\Rastro\\FlexiPlot",
                       QSettings::NativeFormat);
    settings.setValue("Serial/port", ui->serialPort->currentText());
    settings.setValue( "Serial/baudrate", ui->baudrate->currentText() );

    settings.setValue( "UI/tabbed", ui->tabCheckBox->isChecked() );

    emit settingsChanged();

}

//loads settings from the registry
void Settings::loadSettings()
{
    QSettings settings("HKEY_CURRENT_USER\\Software\\Rastro\\FlexiPlot",
                       QSettings::NativeFormat);

    ui->serialPort->setCurrentText( settings.value("Serial/port").toString() );
    ui->baudrate->setCurrentText( settings.value("Serial/baudrate").toString() );

    ui->tabCheckBox->setChecked( settings.value("UI/tabbed").toBool() );

}

Settings::~Settings()
{
    delete ui;
}

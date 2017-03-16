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
    connect(ui->colorBtn, SIGNAL(clicked(bool)), this, SLOT(backgroundColor()));
    loadSettings();
}

void Settings::backgroundColor()
{
    currentBackgroundColor = QColorDialog::getColor(currentBackgroundColor, this);

    if(currentBackgroundColor.isValid())
    {
        ui->colorBtn->setStyleSheet( "QPushButton { background-color:"+ currentBackgroundColor.name() +";\nborder: 1px solid #000; }" );
    }
}


//SAves settings in the registry
void Settings::saveSettings()
{
    QSettings settings;
    settings.setValue( "Serial/port", ui->serialPort->currentText());
    settings.setValue( "Serial/baudrate", ui->baudrate->currentText() );
    settings.setValue( "Serial/data_bits",  ui->dataBits->currentText().toInt() );
    settings.setValue( "Serial/flow_control", (QSerialPort::FlowControl) ui->flowControl->currentIndex() );
    switch(ui->parity->currentIndex())
    {
        case 0: settings.setValue( "Serial/parity", QSerialPort::NoParity ); break;
        case 1: settings.setValue( "Serial/parity", QSerialPort::EvenParity ); break;
        case 2: settings.setValue( "Serial/parity", QSerialPort::OddParity ); break;
        case 3: settings.setValue( "Serial/parity", QSerialPort::SpaceParity ); break;
        case 4: settings.setValue( "Serial/parity", QSerialPort::MarkParity ); break;
        default: ;
    }

    switch(ui->stopBits->currentIndex())
    {
        case 0: settings.setValue( "Serial/stop_bits", QSerialPort::OneStop ); break;
        case 1: settings.setValue( "Serial/stop_bits", QSerialPort::OneAndHalfStop ); break;
        case 2: settings.setValue( "Serial/stop_bits", QSerialPort::TwoStop ); break;
        default: ;
    }

    settings.setValue( "UI/tabbed", ui->tabCheckBox->isChecked() );
    settings.setValue( "UI/backgroundColor", currentBackgroundColor );

    emit settingsChanged();

}

//loads settings from the registry
void Settings::loadSettings()
{
    QSettings settings;

    ui->serialPort->setCurrentText( settings.value("Serial/port").toString() );
    ui->baudrate->setCurrentText( settings.value("Serial/baudrate").toString() );
    ui->dataBits->setCurrentText( QString::number( settings.value("Serial/data_bits").toInt() ) );
    ui->flowControl->setCurrentIndex( settings.value("Serial/flow_control").toInt() );
    switch( settings.value("Serial/parity").toInt() )
    {
        case QSerialPort::NoParity: ui->parity->setCurrentIndex(0); break;
        case QSerialPort::EvenParity: ui->parity->setCurrentIndex(1); break;
        case QSerialPort::OddParity: ui->parity->setCurrentIndex(2); break;
        case QSerialPort::SpaceParity: ui->parity->setCurrentIndex(3); break;
        case QSerialPort::MarkParity: ui->parity->setCurrentIndex(4); break;
        default: ;
    }
    switch(settings.value("Serial/stop_bits").toInt())
    {
        case QSerialPort::OneStop: ui->stopBits->setCurrentIndex(0); break;
        case QSerialPort::OneAndHalfStop: ui->stopBits->setCurrentIndex(1); break;
        case QSerialPort::TwoStop: ui->stopBits->setCurrentIndex(2); break;
        default: ;
    }

    ui->tabCheckBox->setChecked( settings.value("UI/tabbed").toBool() );

    currentBackgroundColor =  QColor( settings.value( "UI/backgroundColor","#a0a0a0" ).toString() );
    ui->colorBtn->setStyleSheet( "QPushButton { background-color:"+ currentBackgroundColor.name() +";\nborder: 1px solid #000; }" );

}

Settings::~Settings()
{
    delete ui;
}

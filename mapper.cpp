#include "mapper.h"
#include "ui_mapper.h"

Mapper::Mapper(QWidget *parent) :
    AbstractWidget(parent),
    ui(new Ui::Mapper)
{
    ui->setupUi(this);

    connect(ui->locEdit, SIGNAL(textEdited(QString)), ui->mapWidget, SLOT(setGMapCenter(QString)));
}

void Mapper::reset()
{

}

void Mapper::serialPacket(QStringList packet)
{

}

QString Mapper::getId()
{
   return id;
}

void Mapper::xmlParse(QXmlStreamReader *xml)
{

}

void Mapper::xmlStream(QXmlStreamWriter *writer)
{

}

void Mapper::setId(QString str)
{
    id = str;
    this->setWindowTitle(QString("%1 \t|\t ID: %2").arg("Sample map plot").arg(str));
//    this->setWindowTitle(QString("%1 \t|\t ID: %2").arg(ui->titleEdit->text()).arg(str));

//    ui->idEdit->blockSignals(true);
//    ui->idEdit->setText( str );
//    ui->idEdit->blockSignals(false);

    Config::getInstance()->setUnsavedChanges(true);
}

Mapper::~Mapper()
{
    delete ui;
}

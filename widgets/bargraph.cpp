#include "bargraph.h"
#include "ui_bargraph.h"

BarGraph::BarGraph(QWidget *parent) :
    AbstractWidget(parent),
    ui(new Ui::BarGraph)
{
    ui->setupUi(this);
}

void BarGraph::serialPacket(QStringList packet)
{

}

void BarGraph::xmlStream(QXmlStreamWriter *writer)
{

}

void BarGraph::xmlParse(QXmlStreamReader *xml)
{

}

void BarGraph::reset()
{

}

void BarGraph::setId(QString str)
{
   id = str;
   this->setWindowTitle(QString("%1 \t|\t ID: %2").arg("ui->titleEdit->text()").arg(str));


   Config::getInstance()->setUnsavedChanges(true);

}

BarGraph::~BarGraph()
{
    delete ui;
}

#include "templatewidget.h"

TemplateWidget::TemplateWidget(QWidget *parent) :
    AbstractWidget(parent)
{
}

bool TemplateWidget::validPacket(QString packet)
{
    return true;
}

void TemplateWidget::reset()
{

}

void TemplateWidget::serialPacket(QStringList packet)
{

}

void TemplateWidget::xmlStream(QXmlStreamWriter *writer)
{

}

void TemplateWidget::xmlParse(QXmlStreamReader *xml)
{

}

void TemplateWidget::setId(QString str)
{
    id = str;
}

void TemplateWidget::setTitle(QString str)
{

}


TemplateWidget::~TemplateWidget()
{
    delete ui;
}

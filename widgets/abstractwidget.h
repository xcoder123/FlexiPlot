#ifndef ABSTRACTWIDGET_H
#define ABSTRACTWIDGET_H

#include <QWidget>
#include <QXmlStreamWriter>
#include <QXmlStreamReader>

class AbstractWidget : public QWidget
{
public:    
    AbstractWidget(QWidget *parent);
    virtual void reset() = 0;
    virtual void serialPacket(QStringList packet) = 0;
    virtual QString getId() = 0;

    virtual int type() const = 0;

    virtual void xmlStream(QXmlStreamWriter *writer) = 0;
    virtual void xmlParse(QXmlStreamReader* xml) = 0;

public slots:
    virtual void setId(QString str) = 0;

};

#endif // ABSTRACTWIDGET_H

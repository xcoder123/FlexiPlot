#ifndef MAPPER_H
#define MAPPER_H

#include <QWidget>
#include "abstractwidget.h"
#include "config.h"


namespace Ui {
class Mapper;
}

class Mapper : public AbstractWidget
{
    Q_OBJECT

public:
    explicit Mapper(QWidget *parent = 0);
    ~Mapper();

    void reset();
    void serialPacket(QStringList packet);
    QString getId();

    void xmlStream(QXmlStreamWriter *writer);
    void xmlParse(QXmlStreamReader* xml);

public slots:
    void setId(QString str);

private:
    Ui::Mapper *ui;

    QString id;
};

#endif // MAPPER_H

#ifndef TEMPLATEWIDGET_H
#define TEMPLATEWIDGET_H

#include <QWidget>
#include <QTime>
#include <QTimer>
#include <QXmlStreamWriter>
#include <QMoveEvent>
#include <QColorDialog>
#include <QRegExpValidator>

#include <QFileDialog>

#include <time.h>

#include <QtCharts>
#include <QtCharts/QChartView>
#include <QtCharts/QAbstractAxis>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>


#include "plotitem.h"
#include "config.h"
#include "abstractwidget.h"

namespace Ui {
class TemplateWidget;
}

class TemplateWidget : public AbstractWidget
{
    Q_OBJECT

public:
    enum {Type = 3 /* must be unique */ };

    int type() const
    {
        return Type;
    }

    explicit TemplateWidget(QWidget *parent = 0);
    ~TemplateWidget();

    void serialPacket(QStringList packet);
    QString getId() { return id; }

    void reset();
    void xmlStream(QXmlStreamWriter *writer);
    void xmlParse(QXmlStreamReader* xml);

    static bool validPacket(QString packet);

public slots:
    void setId(QString str);
    void setTitle(QString str);

private:
    QString id;
};

#endif // TEMPLATEWIDGET_H

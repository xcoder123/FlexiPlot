#ifndef BARGRAPH_H
#define BARGRAPH_H

/*!
 *  Packets:
 *  {P0|Column 1;Column 2;Column 3|Set Name 1|255,0,0|val_col1 val_col2 val_col3|Set Name 2|255,0,0|val_col1 val_col2 val_col3}
 *      Example: {P0|Jan;Feb;Mar|Jane|255,0,0|25 15 17|John|255,0,0|8 9 23}
 *
 *
 *  Colors should be optional, if no color is specified for set, let QtCharts handle it.
 *  {P0|Column 1;Column 2;Column 3|Set Name|val1 val2 val3 val4}
 *
 */

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


#include "plotitem.h"
#include "config.h"
#include "abstractwidget.h"

namespace Ui {
class BarGraph;
}

class BarGraph : public AbstractWidget
{
    Q_OBJECT

public:
    enum { Type = 2 };
    int type() const
    {
        // Enable the use of qgraphicsitem_cast with this item.
        return Type;
    }

    explicit BarGraph(QWidget *parent = 0);
    ~BarGraph();

    void serialPacket(QStringList packet);
    QString getId() { return id; }

    void reset();
    void xmlStream(QXmlStreamWriter *writer);
    void xmlParse(QXmlStreamReader* xml);

    static bool validPacket(QString packet);

public slots:
    void setId(QString str);


private:
    Ui::BarGraph *ui;
    QString id;
};

#endif // BARGRAPH_H

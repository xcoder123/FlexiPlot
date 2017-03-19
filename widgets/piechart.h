#ifndef PIECHART_H
#define PIECHART_H

/*!
 *  Packets:
 *  {P0|Slice 1|255,0,0|value1|Slice 2|0,255,0|value2|Slice 3|0,0,255|value3}
 *      Example: {P0|Slice 1|255,0,0|10|Slice 2|0,255,0|30|Slice 3|0,0,255|20}
 *
 *
 *  Colors should be optional, if no color is specified for set, let QtCharts handle it.
 *  {P0|Slice 1|value1|Slice 2|value2|Slice 3|value3}
 *      Example: {P0|Slice 1|10|Slice 2|30|Slice 3|20}
 *
 */

#include <QWidget>
#include <QTime>
#include <QTimer>
#include <QXmlStreamWriter>
#include <QMoveEvent>
#include <QColorDialog>
#include <QRegExpValidator>
#include <QDebug>

#include <QFileDialog>

#include <time.h>

#include <QtCharts>
#include <QtCharts/QChartView>
#include <QtCharts/QAbstractAxis>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>


#include "flexipieslice.h"

#include "plotitem.h"
#include "config.h"
#include "abstractwidget.h"

namespace Ui {
class PieChart;
}

struct pie_chart_plot_item_t {
    qreal value;
    QString name;
    QColor color;
    bool colorIsSet;
};

class PieChart : public AbstractWidget
{
    Q_OBJECT

public:
    enum {Type = 3 };

    int type() const
    {
        return Type;
    }

    enum LegendPosition {POS_TOP, POS_RIGHT, POS_BOTTOM, POS_LEFT, POS_DETACHED };

    explicit PieChart(QWidget *parent = 0);
    ~PieChart();

    void serialPacket(QStringList packet);
    QString getId() { return id; }

    void reset();
    void xmlStream(QXmlStreamWriter *writer);
    void xmlParse(QXmlStreamReader* xml);

    static bool validPacket(QString packet);


public slots:
    void setId(QString str);
    void setTitle(QString str);
    void settingsChanged();

    void plot();

private:
    void updateDetachedLegendLayout();
    void blockAllSignals(bool block);

private:
    Ui::PieChart *ui;
    QString id;

    QList<pie_chart_plot_item_t> items;

    QChart *chart;
};

#endif // PIECHART_H

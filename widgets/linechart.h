#ifndef LINECHART_H
#define LINECHART_H

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
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>
#include <QtCharts/QDateTimeAxis>


#include "config.h"
#include "abstractwidget.h"
#include "xychartplotitem.h"

#define AUTO_ADD_MARGIN_PERC        1.10        //How much % extra will be added to auto determined axes values


namespace Ui {
class LineChart;
}

//struct line_graph_plot_item_t {
//    QList<QPointF> values;
//    QString series_name;
//    QColor color;
//    bool colorIsSet;
//};



class XYChartPlotItem;

class LineChart : public AbstractWidget
{
    Q_OBJECT

public:
    enum {Type = 1 /* must be unique */ };
    enum LegendPosition {POS_TOP, POS_RIGHT, POS_BOTTOM, POS_LEFT, POS_DETACHED };

    int type() const
    {
        return Type;
    }

    enum PlotType {PLOT_TIME, PLOT_XY};

    explicit LineChart(QWidget *parent = 0);
    ~LineChart();

    void serialPacket(QStringList packet);
    QString getId() { return id; }

    void reset();
    void xmlStream(QXmlStreamWriter *writer);
    void xmlParse(QXmlStreamReader* xml);

    static bool validPacket(QString packet);

private:
    void updateDetachedLegendLayout();


public slots:
    void setId(QString str);
    void setTitle(QString str);
    void settingsChanged();

    void plot();

    void clear();

private slots:
    void samplingSizeChanged(int value);
    void refreshRateChanged(int value);
    void setFileForWriting();
    void startStopAutoWriting(bool start);
    void startStopPlotting();
    void savePlot();
    void blockAllSignals(bool block);

    void legendMarkerClicked();
    void legendMarkerHovered(bool hovering);

private:
    Ui::LineChart *ui;
    QString id;

    QChart *chart;
    QHash<QString, XYChartPlotItem*> items;
    QTimer* timer;

    QDateTimeAxis *dateAxisX;
    QValueAxis* axisY;
    QValueAxis *axisX;

    QFile *autoWriteFile;
    QTextStream autoWriteStream;

    bool doPlotting;
};

#endif // LINECHART_H

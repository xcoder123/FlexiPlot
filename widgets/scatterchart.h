#ifndef SCATTERCHART_H
#define SCATTERCHART_H

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
#include <QtCharts/QScatterSeries>
#include <QtCharts/QValueAxis>

#include "config.h"
#include "abstractwidget.h"
#include "xychartplotitem.h"

namespace Ui {
class ScatterChart;
}

class ScatterChart : public AbstractWidget
{
    Q_OBJECT

public:
    enum LegendPosition {POS_TOP, POS_RIGHT, POS_BOTTOM, POS_LEFT, POS_DETACHED };

    enum {Type = 4 /* must be unique */ };

    int type() const
    {
        return Type;
    }

    explicit ScatterChart(QWidget *parent = 0);
    ~ScatterChart();

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
    void markerPropertiesChanged();
    void samplingSizeChanged(int value);
    void openGlStateChanged();

    void plot();
    void clear();

    void savePlot();
    void startStopPlotting();

private:
    void checkIfAnimationsAllowed();
    void updateDetachedLegendLayout();
    void blockAllSignals(bool b);


private:
    Ui::ScatterChart *ui;
    QString id;

    QChart *chart;
    QHash<QString, XYChartPlotItem*> items;
    bool doPlotting;
};

#endif // SCATTERCHART_H

#ifndef XYCHARTPLOTITEM_H
#define XYCHARTPLOTITEM_H

#include <QtCharts>
#include <QtCharts/QXYSeries>

struct xy_items_max_min_values_t {
    QPointF maxValues;
    QPointF minValues;
};

class XYChartPlotItem
{
public:
    enum UpdateType {
        UPDATE_MANUAL,          //Points are passed to series only when update() method is invoked
        UPDATE_INSTANTLY        //Points are instantly passed to the series
    };

public:
    XYChartPlotItem(QXYSeries *m_series, QString name, UpdateType updateType = UPDATE_MANUAL);

    void addData(QPointF p);
    QVector<QPointF> getData() { return points; }

    void setColor(QColor color);
    QColor getColor() { return color; }
    bool isColorSet() { return colorIsSet; }

    void deleteFirst();
    void clear();

    QXYSeries *series() { return m_series; }

    QPointF maxValues();
    QPointF minValues();

    void setSamplingSize(int size) { m_samplingSize = size; }
    int samplingSize() { return m_samplingSize; }

    void update();

    static xy_items_max_min_values_t getMaxMinValues(QHash<QString, XYChartPlotItem*> items);

private:
    QString name;
    QVector<QPointF> points;
    QColor color;
    QXYSeries *m_series;

    bool colorIsSet;

    int m_samplingSize;

    UpdateType updateType;


};

#endif // XYCHARTPLOTITEM_H

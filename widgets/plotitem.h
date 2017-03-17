#ifndef PLOTITEM_H
#define PLOTITEM_H

#include <qwt_plot_curve.h>
#include <qwt_plot.h>

class PlotItem
{
public:
    PlotItem(QwtPlot * plot, QString name);
    QwtPlotCurve* getCurve() { return curve; }
    void addData(QPointF p, QColor color);
    QVector<QPointF> getData() { return points; }
    QColor getColor() { return color; }
    void deleteFirst();
    void clear();

private:
    QwtPlotCurve* curve;
    QString name;
    QVector<QPointF> points;
    QColor color;
};

#endif // PLOTITEM_H

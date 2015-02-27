#include "plotitem.h"

PlotItem::PlotItem(QwtPlot *plot, QString name)
{
    curve = new QwtPlotCurve(name);

    curve->attach(plot);
    this->name = name;
}

void PlotItem::addData(QPointF p, QColor color)
{
    points.push_back(p);
    this->color = color;
}

void PlotItem::deleteFirst()
{
    points.takeFirst();
}

void PlotItem::clear()
{
    points.clear();
}

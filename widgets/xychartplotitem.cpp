#include "xychartplotitem.h"


XYChartPlotItem::XYChartPlotItem(QXYSeries *m_series, QString name, UpdateType updateType )
{
    this->m_series = m_series;
    this->m_series->setName( name );
    colorIsSet = false;
    this->name = name;
    this->updateType = updateType;

    m_samplingSize = 500;
}

void XYChartPlotItem::update()
{
    m_series->replace( points );
}

void XYChartPlotItem::addData(QPointF p)
{
    switch(updateType)
    {
        case UPDATE_MANUAL:
        //    m_series->append( p );
            if(m_samplingSize == -1)
            {
                points.push_back(p);
                /*if(points.length() > m_samplingSize)
                    this->deleteFirst();*/
            }
            else
            {
                if (points.count() < m_samplingSize)
                {
                    //This is fine, but it is faster for QtCharts to render if we don't have large numbers
                    //since RT line chart can't have nice timestamp, there is no reason not to alter X with sample index
        //            points.push_back( p );

                    points.push_back( QPointF(points.length(), p.y()) );
                }
                else
                {
                    QVector<QPointF> newPoints;
                    for (int i = 0; i < points.count()-1; i++)
                        newPoints.append(QPointF(i, points.at(i+1).y()));

                    newPoints.push_back( QPointF(m_samplingSize, p.y()) );
                    points = newPoints;
                }
            }

            break;

        case UPDATE_INSTANTLY:
            points.append(p); //Do we really need this? Maybe for convinience when getting data. Double the memory though
            m_series->append(p);
            if(m_samplingSize > 0 && m_series->count() > m_samplingSize)
            {
                this->deleteFirst();
                m_series->remove(0);
            }
            break;


        default: qWarning() << "Unknown update type";

    }


}

void XYChartPlotItem::setColor(QColor color)
{
    m_series->setColor( color );
    colorIsSet = true;
    this->color = color;
}

void XYChartPlotItem::deleteFirst()
{
    points.takeFirst();
}

void XYChartPlotItem::clear()
{
    points.clear();
}

QPointF XYChartPlotItem::maxValues()
{
    QPointF result;
    foreach(QPointF p, points)
    {
        if(result.isNull())
        {
            result.setX(p.x());
            result.setY(p.y());
        }
        else
        {
            if(p.x() > result.x() )
                result.setX(p.x());
            if(p.y() > result.y() )
                result.setY(p.y());
        }
    }

    return result;
}

QPointF XYChartPlotItem::minValues()
{
    QPointF result;
    foreach(QPointF p, points)
    {
        if(result.isNull())
        {
            result.setX(p.x());
            result.setY(p.y());
        }
        else
        {
            if(p.x() < result.x() )
                result.setX(p.x());
            if(p.y() < result.y() )
                result.setY(p.y());
        }
    }
    return result;
}

xy_items_max_min_values_t XYChartPlotItem::getMaxMinValues(QHash<QString, XYChartPlotItem*> items)
{
    xy_items_max_min_values_t result;

    QPointF maxValues;
    QPointF minValues;
    foreach(QString key, items.keys())
    {
        XYChartPlotItem * item = items[key];

        if(!item->series()->isVisible())
            continue;

        QPointF itemMaxValues = item->maxValues();

        if(maxValues.isNull())
        {
            maxValues.setX( itemMaxValues.x() );
            maxValues.setY( itemMaxValues.y() );
        }
        else
        {
            if(itemMaxValues.x() > maxValues.x())
                maxValues.setX( itemMaxValues.x() );
            if(itemMaxValues.y() > maxValues.y())
                maxValues.setY( itemMaxValues.y() );
        }


        QPointF itemMinValues = item->minValues();
        if(minValues.isNull())
        {
            minValues.setX( itemMinValues.x() );
            minValues.setY( itemMinValues.y() );
        }
        else
        {
            if(itemMinValues.x() < minValues.x())
                minValues.setX( itemMinValues.x() );
            if(itemMinValues.y() < minValues.y())
                minValues.setY( itemMinValues.y() );
        }
    }

    result.minValues = minValues;
    result.maxValues = maxValues;

    return result;
}

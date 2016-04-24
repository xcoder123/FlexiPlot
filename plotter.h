#ifndef PLOTTER_H
#define PLOTTER_H

#include <QWidget>
#include <QTime>
#include <QTimer>
#include <QXmlStreamWriter>
#include <QMoveEvent>
#include <QColorDialog>
#include <QRegExpValidator>

#include <QFileDialog>

#include <time.h>

#include <qwt_plot.h>
#include <qwt_legend.h>
#include <qwt.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_grid.h>
#include <qwt_date_scale_draw.h>
#include <qwt_date_scale_engine.h>
#include <qwt_text.h>
#include <qwt_scale_widget.h>
#include <qwt_legend_label.h>
#include <qwt_scale_widget.h>
#include <qwt_plot_magnifier.h>
#include <qwt_plot_panner.h>
#include <qwt_plot_canvas.h>
#include <qwt_plot_picker.h>
#include <qwt_picker_machine.h>


#include "plotitem.h"
#include "config.h"
#include "abstractwidget.h"


namespace Ui {
class Plotter;
}

struct graph_t
{
    double x;
    double y;
    QColor color;
    QString name;
    QTime xT;
};

class StockTimeScaleDraw;
class PlotPicker;


class Plotter : public AbstractWidget
{
    Q_OBJECT

public:
    enum PlotType {PLOT_TIME, PLOT_XY};

    explicit Plotter(QWidget *parent = 0);
    ~Plotter();

    void serialPacket(QStringList packet);
    QString getId() { return id; }

    void reset();
    void xmlStream(QXmlStreamWriter *writer);
    void xmlParse(QXmlStreamReader* xml);

public slots:
    void plot();
    void legendChecked( const QVariant &, bool on );
    void showCurve( QwtPlotItem *item, bool on );
    void setId(QString str);
    void setTitle(QString str);
    void setBuffer(int value);
    void setRefreshRate(int ms);
    void setDateFormat(QString format);
    void setXAxisLabel(QString str);
    void setYAxisLabel(QString str);
    void typeChanged(int type);
    void startStopPlotting();    
    void settingsChanged();
    void openColorDialog();
    void savePlot();
    void setFileForWriting();
    void startStopAutoWriting(bool start);

private:
    Ui::Plotter *ui;


    QHash<QString, PlotItem*> plotItems;

    QwtPlotPanner *plotPanner;
    QwtPlotMagnifier *plotMagnifier;

    QString id;
    QTimer *timer;
    double x;
    double minX;

    int buffer;

    bool doPlotting;

    PlotPicker *plotPicker;

    QColor currentColor;

    QFile *autoWriteFile;
    QTextStream autoWriteStream;
};



class StockTimeScaleDraw: public QwtScaleDraw
{
  public:
        StockTimeScaleDraw(QString fmt) : format(fmt) { }

    virtual QwtText label(double v) const
    {
        QDateTime t = QDateTime::fromMSecsSinceEpoch(v);
        return t.toString(format);
    }
 private:
    const QString format;
};

class PlotPicker : QwtPlotPicker
{
public:
    PlotPicker( QWidget *canvas, QString format ):
        QwtPlotPicker( canvas )
    {
        setRubberBand( QwtPlotPicker::NoRubberBand );
        setTrackerMode( QwtPlotPicker::AlwaysOn );
        this->format = format;
        this->date = true;

        this->setStateMachine( new QwtPickerClickPointMachine() );

    }

    void setFormat(QString f) { format = f; }
    void showDate(bool bb) { date = bb; }

protected:
    virtual QwtText trackerTextF( const QPointF &pos ) const
    {

        const QDateTime dt = QwtDate::toDateTime( pos.x() );

        QString s;

        if(date)
        {
            s += QDateTime::fromMSecsSinceEpoch(pos.x()).toString(format) + " | " + QString::number(pos.y());
        }
        else
            s += QString::number(pos.x()) + " | " + QString::number(pos.y());

        QwtText text( s );
        text.setColor( Qt::white );

        QColor c = rubberBandPen().color();
        text.setBorderPen( QPen( c ) );
        text.setBorderRadius( 2 );
        c.setAlpha( 170 );
        text.setBackgroundBrush( c );

        return text;
    }



private:
    QString format;
    bool date;
};



#endif // PLOTTER_H

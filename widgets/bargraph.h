#ifndef BARGRAPH_H
#define BARGRAPH_H

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
class BarGraph;
}

class BarGraph : public AbstractWidget
{
    Q_OBJECT

public:
    explicit BarGraph(QWidget *parent = 0);
    ~BarGraph();

    void serialPacket(QStringList packet);
    QString getId() { return id; }

    void reset();
    void xmlStream(QXmlStreamWriter *writer);
    void xmlParse(QXmlStreamReader* xml);

public slots:
    void setId(QString str);


private:
    Ui::BarGraph *ui;
    QString id;
};

#endif // BARGRAPH_H

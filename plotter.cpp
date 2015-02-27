#include "plotter.h"
#include "ui_plotter.h"

Plotter::Plotter(QWidget *parent) :
    AbstractWidget(parent),
    ui(new Ui::Plotter)
{
    ui->setupUi(this);

    this->setWindowTitle("Plot 1");
    doPlotting = true;

    timer = new QTimer(this);
    timer->setInterval(50);
    timer->start();
    connect(timer, SIGNAL(timeout()), this, SLOT(plot()));

    //QwtPlot* plot = new QwtPlot(this);

    ui->plot->setAutoFillBackground( true );
    ui->plot->setPalette( QPalette( QColor( 255, 255, 255 ) ) );

    ui->plot->setTitle( ui->titleEdit->text() );
    ui->plot->insertLegend( new QwtLegend(), QwtPlot::RightLegend );

    ui->plot->setMinimumSize(100,100);


    ui->plot->setAxisScaleDraw ( QwtPlot::xBottom, new StockTimeScaleDraw(ui->dateFormat->text()) );
    //ui->plot->setAxisScaleEngine ( QwtPlot::xBottom, new QwtDateScaleEngine ( Qt::UTC ) );

    QwtScaleWidget *scaleWidget = ui->plot->axisWidget( QwtPlot::xBottom );
    const int fmh = QFontMetrics( scaleWidget->font() ).width(ui->dateFormat->text());
    scaleWidget->setMinBorderDist( 0, fmh/2);

    QwtLegend *legend = new QwtLegend;
    legend->setDefaultItemMode( QwtLegendData::Checkable );
    ui->plot->insertLegend( legend, QwtPlot::TopLegend );

    connect( legend, SIGNAL( checked( const QVariant &, bool, int ) ),
        SLOT( legendChecked( const QVariant &, bool ) ) );


    this->reset();



    //Graphics items

    connect(ui->idEdit, SIGNAL(textEdited(QString)), this, SLOT(setId(QString)));
    connect(ui->titleEdit, SIGNAL(textEdited(QString)), this, SLOT(setTitle(QString)));

    buffer = 100;
    connect(ui->bufferSpin, SIGNAL(valueChanged(int)), this, SLOT(setBuffer(int)));
    connect(ui->refreshRateSpin, SIGNAL(valueChanged(int)), this, SLOT(setRefreshRate(int)));

    connect(ui->dateFormat, SIGNAL(textEdited(QString)), this, SLOT(setDateFormat(QString)));
    connect(ui->xaxisEdit,SIGNAL(textEdited(QString)), this, SLOT(setXAxisLabel(QString)));
    connect(ui->yaxisEdit, SIGNAL(textEdited(QString)), this, SLOT(setYAxisLabel(QString)));

    connect(ui->typeComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(typeChanged(int)));
    connect(ui->startStopPlotBtn, SIGNAL(clicked(bool)), this, SLOT(startStopPlotting()));

    connect(ui->autoScaleXCheckBox, SIGNAL(clicked()), this, SLOT(settingsChanged()));
    connect(ui->minXSpin,SIGNAL(editingFinished()), this, SLOT(settingsChanged()));
    connect(ui->maxXSpin, SIGNAL(editingFinished()), this, SLOT(settingsChanged()));

    connect(ui->autoScaleYCheckBox, SIGNAL(clicked()), this, SLOT(settingsChanged()));
    connect(ui->minYSpin,SIGNAL(editingFinished()), this, SLOT(settingsChanged()));
    connect(ui->maxYSpin, SIGNAL(editingFinished()), this, SLOT(settingsChanged()));

    connect(ui->fillCheckBox, SIGNAL(toggled(bool)), this, SLOT(settingsChanged()));
    connect(ui->transparentSpinBox, SIGNAL(editingFinished()), this, SLOT(settingsChanged()));

    connect(ui->colorBtn, SIGNAL(clicked()), this, SLOT(openColorDialog()));




    // canvas
    QwtPlotCanvas *canvas = new QwtPlotCanvas();
    canvas->setLineWidth( 1 );
    canvas->setFrameStyle( QFrame::Box | QFrame::Plain );
    canvas->setBorderRadius( 15 );

    QPalette canvasPalette( Qt::white );
    canvasPalette.setColor( QPalette::Foreground, QColor( 133, 190, 232 ) );
    canvas->setPalette( canvasPalette );

    ui->plot->setCanvas( canvas );

    plotPanner = new QwtPlotPanner(canvas);
    plotMagnifier = new QwtPlotMagnifier(canvas);


    plotPicker = new PlotPicker(canvas, ui->dateFormat->text());

    currentColor = Qt::white;




}



void Plotter::openColorDialog()
{
    currentColor = QColorDialog::getColor(currentColor, this);

    if(currentColor.isValid())
    {
        ui->colorLbl->setStyleSheet( "background-color:"+ currentColor.name() +";\nborder: 2px solid #000;" );

        ui->plot->setCanvasBackground( QBrush(currentColor) );
    }

    Config::getInstance()->setUnsavedChanges(true);
}

void Plotter::settingsChanged()
{
    Config::getInstance()->setUnsavedChanges(true);
    plot();
}

void Plotter::startStopPlotting()
{
    if(ui->startStopPlotBtn->isChecked())
    {
        doPlotting = true;
        timer->start();
        plotMagnifier->setAxisEnabled(QwtPlot::xBottom, false);
        plotMagnifier->setAxisEnabled(QwtPlot::yLeft, false);
        plot();
    }
    else
    {
        doPlotting = false;
        timer->stop();
        plotMagnifier->setAxisEnabled(QwtPlot::xBottom, true);
        plotMagnifier->setAxisEnabled(QwtPlot::yLeft, true);
    }
}

void Plotter::typeChanged(int type)
{
    if(type == Plotter::PLOT_TIME)
    {
        ui->plot->setAxisScaleDraw ( QwtPlot::xBottom, new StockTimeScaleDraw(ui->dateFormat->text()) );
        //ui->plot->setAxisScaleEngine ( QwtPlot::xBottom, new QwtDateScaleEngine ( Qt::UTC ) );

        QwtScaleWidget *scaleWidget = ui->plot->axisWidget( QwtPlot::xBottom );
        const int fmh = QFontMetrics( scaleWidget->font() ).width(ui->dateFormat->text());
        scaleWidget->setMinBorderDist( 0, fmh/2);
    }
    else if(type == Plotter::PLOT_XY)
    {
        ui->plot->setAxisScaleDraw( QwtPlot::xBottom, new QwtScaleDraw() );
    }
}

void Plotter::reset()
{
    ui->plot->detachItems();


    QwtPlotGrid *grid = new QwtPlotGrid();
    grid->setPen( Qt::gray, 0.0, Qt::DotLine );
    grid->enableX( true );
    grid->enableXMin( true );
    grid->enableY( true );
    grid->enableYMin( false );
    grid->attach( ui->plot );


    x = QDateTime::currentMSecsSinceEpoch();
    minX = QDateTime::currentMSecsSinceEpoch();

    this->plotItems.clear();

    ui->plot->replot();
}

void Plotter::setXAxisLabel(QString str)
{
    ui->plot->setAxisTitle( QwtPlot::xBottom, str );
    Config::getInstance()->setUnsavedChanges(true);
}

void Plotter::setYAxisLabel(QString str)
{
    ui->plot->setAxisTitle( QwtPlot::yLeft, str );
    Config::getInstance()->setUnsavedChanges(true);
}

void Plotter::setDateFormat(QString format)
{
     ui->plot->setAxisScaleDraw ( QwtPlot::xBottom, new StockTimeScaleDraw(format) );

     QwtScaleWidget *scaleWidget = ui->plot->axisWidget( QwtPlot::xBottom );
     const int fmh = QFontMetrics( scaleWidget->font() ).width(format);
     scaleWidget->setMinBorderDist( 0, fmh/2);

     plotPicker->setFormat(format);
     Config::getInstance()->setUnsavedChanges(true);
}

void Plotter::setRefreshRate(int ms)
{
    timer->setInterval(ms);
    Config::getInstance()->setUnsavedChanges(true);
}

void Plotter::setBuffer(int value)
{
    if(value < buffer)
    {
        foreach(PlotItem *item, plotItems)
        {
            item->clear();
        }
        x = QDateTime::currentMSecsSinceEpoch();
        minX = QDateTime::currentMSecsSinceEpoch();
    }
    buffer = value;
    Config::getInstance()->setUnsavedChanges(true);


}

void Plotter::setTitle(QString str)
{
    ui->plot->setTitle( str );
    this->setWindowTitle(QString("%1 \t|\t ID: %2").arg(str).arg(id));
    Config::getInstance()->setUnsavedChanges(true);
}

void Plotter::legendChecked( const QVariant &itemInfo, bool on )
{
    QwtPlotItem *plotItem = ui->plot->infoToItem( itemInfo );
    if ( plotItem )
        showCurve( plotItem, on );
}


void Plotter::plot()
{
    //ui->plot->detachItems();


    //SCALING CHECK!
    if(ui->typeComboBox->currentIndex() == Plotter::PLOT_TIME)
    {
        ui->plot->setAxisAutoScale( QwtPlot::xBottom, false );
        ui->plot->setAxisScale( QwtPlot::xBottom, minX, x );

        if(ui->autoScaleYCheckBox->isChecked())
        {
            ui->plot->setAxisAutoScale( QwtPlot::yLeft, true );
        }
        else
        {
            ui->plot->setAxisAutoScale( QwtPlot::yLeft, false);
            ui->plot->setAxisScale( QwtPlot::yLeft, ui->minYSpin->value(), ui->maxYSpin->value() );
        }

        plotPicker->showDate( true );
    }
    else
    {
        if(ui->autoScaleYCheckBox->isChecked())
        {
            ui->plot->setAxisAutoScale( QwtPlot::yLeft, true );
        }
        else
        {
            ui->plot->setAxisAutoScale( QwtPlot::yLeft, false);
            ui->plot->setAxisScale( QwtPlot::yLeft, ui->minYSpin->value(), ui->maxYSpin->value() );
        }

        if(ui->autoScaleXCheckBox->isChecked())
        {
            ui->plot->setAxisAutoScale( QwtPlot::xBottom, true );
        }
        else
        {
            ui->plot->setAxisAutoScale( QwtPlot::xBottom, false);
            ui->plot->setAxisScale( QwtPlot::xBottom, ui->minXSpin->value(), ui->maxXSpin->value() );
        }


        plotPicker->showDate( false );
    }
    //END SCALING CHECK




    foreach(QString key, plotItems.keys())
    {
        //QwtPlotCurve *curve = new QwtPlotCurve( graphs[key].last().name );

        QwtPlotCurve *curve = plotItems[key]->getCurve();
        curve->setRenderHint( QwtPlotItem::RenderAntialiased );
        curve->setPen( QPen( plotItems[key]->getColor() )  );
        curve->setLegendAttribute( QwtPlotCurve::LegendShowLine );

        int red =  plotItems[key]->getColor().red()-10 < 0 ? 0 :  plotItems[key]->getColor().red()-10;
        int green =  plotItems[key]->getColor().green()-10 < 0 ? 0 :  plotItems[key]->getColor().green()-10;
        int blue =  plotItems[key]->getColor().blue()-10 < 0 ? 0 :  plotItems[key]->getColor().blue()-10;

        if(ui->fillCheckBox->isChecked())
        {
            curve->setBrush( QBrush(QColor(red,
                                                    green,
                                                    blue,
                                                    ui->transparentSpinBox->value())));
        }
        else
            curve->setBrush( Qt::NoBrush );

        QVector<QPointF> data;



        for (int i=0; i<plotItems[key]->getData().size(); ++i)
        {
            data << plotItems[key]->getData().at(i);

        }

        curve->setSamples(data);
        //curve->attach(ui->plot);
    }

    ui->plot->replot();
}

void Plotter::showCurve( QwtPlotItem *item, bool on )
{
    item->setVisible( on );

    QwtLegend *lgd = qobject_cast<QwtLegend *>( ui->plot->legend() );

    QList<QWidget *> legendWidgets =
        lgd->legendWidgets( ui->plot->itemToInfo( item ) );

    if ( legendWidgets.size() == 1 )
    {
        QwtLegendLabel *legendLabel =
            qobject_cast<QwtLegendLabel *>( legendWidgets[0] );

        if ( legendLabel )
            legendLabel->setChecked( on );
    }

    ui->plot->replot();
}


void Plotter::serialPacket(QStringList packet)
{

    if(!doPlotting)
        return;

//    qDebug() << this->id << packet;

    if(ui->typeComboBox->currentIndex() == Plotter::PLOT_TIME)
    {        
        x = QDateTime::currentMSecsSinceEpoch();

        for(int i=1;i<packet.size(); i+=3)
        {
            QRegExp rx("\\d{1,},\\d{1,},\\d{1,}");
            QRegExpValidator v(rx,this);

            if(!packet[i+1].contains(rx))
            {
                qWarning() << "Incorrect color";
                return;
            }

            QStringList color = packet[i+1].split(",");

            if(!plotItems.keys().contains(packet[i])) //does it have the current item?
            {
               PlotItem* plotItem = new PlotItem(ui->plot, packet[i]);
               plotItems.insert(packet[i], plotItem);
               showCurve(plotItem->getCurve(), true);
            }

            PlotItem* plotItem = plotItems[packet[i]];
            plotItem->addData( QPointF(x, packet[i+2].toDouble()),
                    QColor(color[0].toInt(),
                                    color[1].toInt(),
                                    color[2].toInt() )
            );
//            qDebug() << packet[i] << packet[i+1] << packet[i+2];

            if(plotItem->getData().size() > buffer)
            {
                plotItem->deleteFirst();
                minX = plotItem->getData().first().x();
            }


        }

    }
    else if(ui->typeComboBox->currentIndex() == Plotter::PLOT_XY)
    {
        //qDebug() << packet;
        for(int i=1;i<packet.size(); i+=3)
        {
            QStringList color = packet[i+1].split(",");

            if(!plotItems.keys().contains(packet[i])) //does it have the current item?
            {
               PlotItem* plotItem = new PlotItem(ui->plot, packet[i]);
               plotItems.insert(packet[i], plotItem);
               showCurve(plotItem->getCurve(), true);
            }

            PlotItem* plotItem = plotItems[packet[i]];

            QStringList xy = packet[i+2].split(" ");
            if(xy.last() == "")
                xy.pop_back();

            if(xy.size() % 2 == 0)
            {
                plotItem->clear();
                for(int g=0; g<xy.size(); g+=2)
                {
                    plotItem->addData( QPointF(xy[g].toDouble(), xy[g+1].toDouble()),
                            QColor(color[0].toInt(),
                                            color[1].toInt(),
                                            color[2].toInt() )
                    );
                }

               // qDebug() << plotItem->getData() << plotItem->getData().size();
            }

//            if(plotItem->getData().size() > buffer)
//            {
//                plotItem->deleteFirst();
//                minX = plotItem->getData().first().x();
//            }


        }
    }

}

void Plotter::setId(QString str)
{
   id = str;
   this->setWindowTitle(QString("%1 \t|\t ID: %2").arg(ui->titleEdit->text()).arg(str));

   ui->idEdit->blockSignals(true);
   ui->idEdit->setText( str );
   ui->idEdit->blockSignals(false);

   Config::getInstance()->setUnsavedChanges(true);

}

void Plotter::xmlStream(QXmlStreamWriter *writer)
{
    writer->writeTextElement("id", this->id);
    writer->writeTextElement("title", ui->titleEdit->text());
    writer->writeTextElement("xname", ui->xaxisEdit->text());
    writer->writeTextElement("yname", ui->yaxisEdit->text());
    writer->writeTextElement("type", QString::number(ui->typeComboBox->currentIndex()));
    writer->writeTextElement("buffer", QString::number(ui->bufferSpin->value()));
    writer->writeTextElement("refresh_rate", QString::number(ui->refreshRateSpin->value()));
    writer->writeTextElement("auto_scale_y", QString::number(ui->autoScaleYCheckBox->isChecked()));
    writer->writeTextElement("min_scale_y", QString::number(ui->minYSpin->value()));
    writer->writeTextElement("max_scale_y", QString::number(ui->maxYSpin->value()));
    writer->writeTextElement("auto_scale_x", QString::number(ui->autoScaleXCheckBox->isChecked()));
    writer->writeTextElement("min_scale_x", QString::number(ui->minXSpin->value()));
    writer->writeTextElement("max_scale_x", QString::number(ui->maxXSpin->value()));
    writer->writeTextElement("date_format", ui->dateFormat->text());
    writer->writeTextElement("auto_filling", QString::number(ui->fillCheckBox->isChecked()));
    writer->writeTextElement( "background-color", currentColor.name() );
    writer->writeTextElement( "auto_fill_opacity", QString::number(ui->transparentSpinBox->value()) );
}

void Plotter::xmlParse(QXmlStreamReader *xml)
{
    /* Let's check that we're really getting a person. */
    if(xml->tokenType() != QXmlStreamReader::StartElement &&
            xml->name() == "Plotter") {
        return;
    }

    /* Next element... */
    xml->readNext();

    while(!(xml->tokenType() == QXmlStreamReader::EndElement && xml->name() == "Plotter"))
    {
        if(xml->name() == "WindowGeometry")
        {
            QXmlStreamAttributes attributes = xml->attributes();

            if(attributes.hasAttribute("x") && attributes.hasAttribute("y")
                    && attributes.hasAttribute("width") && attributes.hasAttribute("height"))
            {
                this->parentWidget()->setGeometry(
                                    attributes.value("x").toInt(),
                                    attributes.value("y").toInt(),
                                    attributes.value("width").toInt(),
                                    attributes.value("height").toInt()
                                );
            }

        }

        if(xml->name() == "id")
        {
            QString str = xml->readElementText();
            id = str;
            ui->idEdit->setText(str);
            //this->setWindowTitle(QString("Plot: %1").arg(str));
        }

        if(xml->name() == "title")
        {
            QString str = xml->readElementText();
            ui->titleEdit->setText(str);
            ui->plot->setTitle(str );

            this->setWindowTitle(QString("%1 \t|\t ID: %2").arg(ui->titleEdit->text()).arg(ui->idEdit->text()));
        }

        if(xml->name() == "xname")
        {
            QString str = xml->readElementText();
            ui->xaxisEdit->setText(str);
            ui->plot->setAxisTitle(QwtPlot::xBottom, str);
        }

        if(xml->name() == "yname")
        {
            QString str = xml->readElementText();
            ui->yaxisEdit->setText(str);
            ui->plot->setAxisTitle(QwtPlot::yLeft, str);
        }

                if(xml->name() == "type")
                {
                    int type = xml->readElementText().toInt();
                    ui->typeComboBox->setCurrentIndex(type);
                    typeChanged(type);
                }

                if(xml->name() == "buffer")
                {
                    int value = xml->readElementText().toInt();
                    ui->bufferSpin->setValue( value );
                    if(value < buffer)
                    {
                        foreach(PlotItem *item, plotItems)
                        {
                            item->clear();
                        }
                        x = QDateTime::currentMSecsSinceEpoch();
                        minX = QDateTime::currentMSecsSinceEpoch();
                    }
                    buffer = value;
                }

                if(xml->name() == "refresh_rate")
                {
                    int rate = xml->readElementText().toInt();
                    ui->refreshRateSpin->setValue(rate);
                    timer->setInterval(rate);
                }

                if(xml->name() == "auto_scale_y")
                    ui->autoScaleYCheckBox->setChecked(xml->readElementText().toInt());

                if(xml->name() == "min_scale_y")
                    ui->minYSpin->setValue( xml->readElementText().toDouble() );

                if(xml->name() == "max_scale_y")
                    ui->maxYSpin->setValue( xml->readElementText().toDouble() );

                if(xml->name() == "auto_scale_x")
                    ui->autoScaleXCheckBox->setChecked(xml->readElementText().toInt());

                if(xml->name() == "min_scale_x")
                    ui->minXSpin->setValue( xml->readElementText().toDouble() );

                if(xml->name() == "max_scale_x")
                    ui->maxXSpin->setValue( xml->readElementText().toDouble() );

                if(xml->name() == "date_format")
                {
                    QString format = xml->readElementText();
                    ui->dateFormat->setText(format);

                    if(this->ui->typeComboBox->currentIndex() == PLOT_TIME)
                    {
                        ui->plot->setAxisScaleDraw ( QwtPlot::xBottom, new StockTimeScaleDraw(format) );

                        QwtScaleWidget *scaleWidget = ui->plot->axisWidget( QwtPlot::xBottom );
                        const int fmh = QFontMetrics( scaleWidget->font() ).width(format);
                        scaleWidget->setMinBorderDist( 0, fmh/2);

                        plotPicker->setFormat(format);
                    }
                }

                if(xml->name() == "auto_filling")
                    ui->fillCheckBox->setChecked( xml->readElementText().toInt() );

                if(xml->name() == "background-color")
                {
                    QString str = xml->readElementText();
                    currentColor = QColor(str);

                    ui->colorLbl->setStyleSheet( "background-color:"+ currentColor.name() +";\nborder: 2px solid #000;" );

                    ui->plot->setCanvasBackground( QBrush(currentColor) );
                }

                if(xml->name() == "auto_fill_opacity")
                    ui->transparentSpinBox->setValue( xml->readElementText().toInt() );


        /* ...and next... */
        xml->readNext();
    }

}

Plotter::~Plotter()
{
    delete ui;
}

#include "linechart.h"
#include "ui_linechart.h"

LineChart::LineChart(QWidget *parent) :
    AbstractWidget(parent),
    ui(new Ui::LineChart)
{
    ui->setupUi(this);

    doPlotting = true;

    autoWriteFile = NULL;

    chart = new QChart();
    chart->setTitle( ui->titleEdit->text() );
    QFont tFont = chart->titleFont();
    tFont.setPointSizeF( 15 );
    tFont.setBold( true );
    chart->setTitleFont( tFont );


    axisX = new QValueAxis;
    axisX->setTickCount(10);
    axisX->setLabelFormat("%d");
    axisX->setTitleText("Samples");
    axisX->setLabelsVisible(true);

//    dateAxisX = new QDateTimeAxis;
//    dateAxisX->setTickCount(10);
//    dateAxisX->setFormat("h:m:s");
//    dateAxisX->setTitleText("Time");

    axisY = new QValueAxis;
    axisY->setRange(-1, 1);    
    chart->setAxisX(axisX);
    chart->setAxisY(axisY);


    ui->chartView->setChart( chart );
    ui->chartView->setRenderHint(QPainter::Antialiasing);

    this->setWindowTitle(ui->titleEdit->text());

    connect(ui->clearBtn, SIGNAL(clicked(bool)), this, SLOT(clear()));
    connect(ui->startStopPlotBtn, SIGNAL(clicked(bool)), this, SLOT(startStopPlotting()));
    connect(ui->saveBtn, SIGNAL(clicked(bool)), this, SLOT(savePlot()));

    connect(ui->idEdit, SIGNAL(textEdited(QString)), this, SLOT(setId(QString)));
    connect(ui->titleEdit, SIGNAL(textEdited(QString)), this, SLOT(setTitle(QString)));
    connect(ui->nameXaxis, SIGNAL(textEdited(QString)), this, SLOT(settingsChanged()));
    connect(ui->nameYaxis, SIGNAL(textEdited(QString)), this, SLOT(settingsChanged()));

    connect(ui->animationComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(settingsChanged()));
    connect(ui->themeComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(settingsChanged()));
    connect(ui->antialiasingCheckBox, SIGNAL(stateChanged(int)), this, SLOT(settingsChanged()));

    ui->legendDetachedParamGroup->setVisible(false);
    connect(ui->legendShowCheckBox, SIGNAL(stateChanged(int)), this, SLOT(settingsChanged()));
    connect(ui->legendBoldCheckBox, SIGNAL(clicked(bool)), this, SLOT(settingsChanged()));
    connect(ui->legendItalicCheckBox, SIGNAL(clicked(bool)), this, SLOT(settingsChanged()));
    connect(ui->legendUnderlineCheckBox, SIGNAL(clicked(bool)), this, SLOT(settingsChanged()));
    connect(ui->legendFontSizeSpinBox, SIGNAL(valueChanged(double)), this, SLOT(settingsChanged()));
    connect(ui->legendFontComboBox, SIGNAL(currentFontChanged(QFont)), this, SLOT(settingsChanged()));
    connect(ui->legendPositionComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(settingsChanged()));
    connect(ui->legendHPosSpinBox, SIGNAL(valueChanged(double)), this, SLOT(settingsChanged()));
    connect(ui->legendVPosSpinBox, SIGNAL(valueChanged(double)), this, SLOT(settingsChanged()));
    connect(ui->legendWidthSpinBox, SIGNAL(valueChanged(double)), this, SLOT(settingsChanged()));
    connect(ui->legendHeightSpinBox, SIGNAL(valueChanged(double)), this, SLOT(settingsChanged()));



    connect(ui->bufferSpin, SIGNAL(valueChanged(int)), this, SLOT(samplingSizeChanged(int)));
    connect(ui->refreshRateSpin, SIGNAL(valueChanged(int)), this, SLOT(refreshRateChanged(int)));


    connect(ui->xAxisTickCountSpinBox, SIGNAL(valueChanged(int)), this, SLOT(settingsChanged()));
    connect(ui->yAxisTickCountSpinBox, SIGNAL(valueChanged(int)), this, SLOT(settingsChanged()));

    connect(ui->openForWriteBtn, SIGNAL(clicked(bool)), this, SLOT(setFileForWriting()));
    connect(ui->writeToFileCheckBox, SIGNAL(clicked(bool)), this, SLOT(startStopAutoWriting(bool)));

    timer = new QTimer(this);
    timer->setInterval(50);
    timer->start();
    connect(timer, SIGNAL(timeout()), this, SLOT(plot()));

}

bool LineChart::validPacket(QString packet)
{

    /* Regular expresion for real time plotting:
     * Color Test cases:
     *             P0|X|255,0,0|100|Y|0,255,0|150|Z|0,0,255|200
     *
     * Regexp:
     *             [a-zA-Z0-9]+(\|[a-zA-Z0-9 ]+\|\d{1,3},\d{1,3},\d{1,3}\|(\-*\d+(\.{0,1}\d+)*)*)+
     *
     *
     * No Color Test Cases:
     *              P0|X|100|Y|150|Z|200
     *
     * Regexp:
     *              [a-zA-Z0-9]+(\|[a-zA-Z0-9 ]+\|(\-*\d+(\.{0,1}\d+)*)*)+
     *
     *
     *
     *
     * Regular expression for XY plot
     * Color test case:
     *              P0|Random|255,0,0|1 255 2 200 3 150 4 100|Quadratic|0,255,0|-5 25 -4 16 -3 9 -2 4 -1 1 0 0 1 1 2 4 3 9 4 16 5 25
     *
     * Regexp:
     *              [a-zA-Z0-9]+(\|[a-zA-Z0-9 ]+\|\d{1,3},\d{1,3},\d{1,3}\|(\-*\d+(\.{0,1}\d+)*\s?)*)+
     *
     *
     * No Color Test Cases:
     *             P0|Random|1 255 2 200 3 150 4 100|Quadratic|-5 25 -4 16 -3 9 -2 4 -1 1 0 0 1 1 2 4 3 9 4 16 5 25
     *
     * Regexp:
     *             [a-zA-Z0-9]+(\|[a-zA-Z0-9 ]+\|(\-*\d+(\.{0,1}\d+)*\s?)*)+
     *
     */

    QRegExp rtFullPacket("[a-zA-Z0-9]+(\\|[a-zA-Z0-9 ]+\\|\\d{1,3},\\d{1,3},\\d{1,3}\\|(\\-*\\d+(\\.{0,1}\\d+)*)*)+");
    QRegExp rtNoColorPacket("[a-zA-Z0-9]+(\\|[a-zA-Z0-9 ]+\\|(\\-*\\d+(\\.{0,1}\\d+)*)*)+");
    QRegExp xyFullPacket("[a-zA-Z0-9]+(\\|[a-zA-Z0-9 ]+\\|\\d{1,3},\\d{1,3},\\d{1,3}\\|(\\-*\\d+(\\.{0,1}\\d+)*\\s?)*)+");
    QRegExp xyNoColorPacket("[a-zA-Z0-9]+(\\|[a-zA-Z0-9 ]+\\|(\\-*\\d+(\\.{0,1}\\d+)*\\s?)*)+");

    return rtFullPacket.exactMatch(packet) || rtNoColorPacket.exactMatch(packet) ||
            xyFullPacket.exactMatch(packet) || xyNoColorPacket.exactMatch(packet);
}

void LineChart::reset()
{
    foreach(QString key, items.keys())
        delete items.value(key);
    items.clear();

    chart->removeAllSeries();
}


void LineChart::clear()
{
    reset();
}

/*! IMPORTANT EXCERPT FROM DOCUMENTATION
 *
 * Note: A newly added series is attached to no axes by default, including any axes that were created
 * for the chart using createDefaultAxes() before the series was added to the chart. If no axes are
 * attached to the newly added series before the chart is shown, the series will get drawn as if it
 * had axes with ranges that exactly fit the series to the plot area of the chart. This can be
 * confusing if the same chart also displays other series that have properly attached axes, so always
 * make sure you either call createDefaultAxes() after a series has been added or explicitly attach axes
 * for the series.
 */
void LineChart::serialPacket(QStringList packet)
{

    if(doPlotting == false)
        return;

    if(ui->typeComboBox->currentIndex() == PLOT_XY)
    {
        reset();
    }


    //This here is only applicable to RT plot. Unfortunatelly, I can't pass 64bit epoch for nice timestamp yet.
    //It is due to limitation in OpenGL and Qt. OpenGL by default only supports 32bit floats, and Qt hasn't implemented
    //a "simulated" double float or is not using GL version 4+.
//    qint64 currentMillis = QDateTime::currentMSecsSinceEpoch();
    static qint64 currentPointIndex = 0;
    currentPointIndex++;

    bool autoWriteSkipFirstSeperator = true; //Used to skip adding sperator for first value

    for(int i=1; i<packet.length(); i++)
    {

        QLineSeries * series = NULL;
        if(!items.keys().contains(packet[i])) //does it have the current item?
        {
           series =  new QLineSeries();
           series->setUseOpenGL(true);
           chart->addSeries( series );
           series->attachAxis( axisX );
           series->attachAxis( axisY );

           foreach(QLegendMarker* marker, chart->legend()->markers())
           {
               disconnect(marker, SIGNAL(clicked()), this, SLOT(legendMarkerClicked()));
               disconnect(marker, SIGNAL(hovered(bool)), this, SLOT(legendMarkerHovered(bool)));

               connect(marker, SIGNAL(clicked()), this, SLOT(legendMarkerClicked()));
               connect(marker, SIGNAL(hovered(bool)), this, SLOT(legendMarkerHovered(bool)));
           }


           LineChartPlotItem* plotItem = new LineChartPlotItem(series, packet[i]);

           if(ui->typeComboBox->currentIndex() == PLOT_TIME)
               plotItem->setSamplingSize( ui->bufferSpin->value() );
           else
               plotItem->setSamplingSize( -1 ); //-1 = no buffer, just add the damn point

           items.insert(packet[i], plotItem);
        }

        LineChartPlotItem* plotItem = items[packet[i]];


        QRegExp rx("\\d{1,},\\d{1,},\\d{1,}");
        QRegExpValidator v(rx,this);

        QStringList values;
        if(packet[i+1].contains(rx)) //This set has a color specified
        {
            QStringList color = packet[i+1].split(",");
            plotItem->setColor( QColor(color[0].toInt(),color[1].toInt(),color[2].toInt()) );

            values = packet[i+2].split(" ");
            i += 2;
        }
        else
        {
            values = packet[i+1].split(" ");
            i += 1;
        }

//        qDebug() << values;

        for(int vI=0; vI<values.length(); vI += 2)
        {
            if(vI+1 >= values.length())
            {
//                qDebug() << "Adding RT point" << QPointF( (double)currentMillis, values.at(vI).toDouble() );
                plotItem->addData( QPointF( (double)currentPointIndex, values.at(vI).toDouble() ) );
            }
            else
            {
//                qDebug() << "Adding XY point " << QPointF( values.at(vI).toDouble(), values.at(vI+1).toDouble() );
                plotItem->addData( QPointF( values.at(vI).toDouble(), values.at(vI+1).toDouble() ) );
            }

            //Auto write to file the packet, if opened
            if(autoWriteFile != NULL && autoWriteFile->isOpen() && ui->writeToFileCheckBox->isChecked())
            {
                if(autoWriteSkipFirstSeperator == false)
                    autoWriteStream << ui->seperatorEdit->text();
                else
                    autoWriteSkipFirstSeperator = false;

                autoWriteStream << plotItem->getData().last().x() << ui->seperatorEdit->text() << plotItem->getData().last().y();
            }

        }
    }

    if(autoWriteFile != NULL && autoWriteFile->isOpen() && ui->writeToFileCheckBox->isChecked())
    {
        autoWriteStream << endl;
    }
}


void LineChart::plot()
{
    foreach(QString key, items.keys())
    {
//        qDebug() << key << items[key]->series()->useOpenGL() << items[key]->getData();
        items[key]->update();
    }


    line_plot_max_min_values_t minMaxGraphValues;
    if(ui->autoScaleXCheckBox->isChecked() || ui->autoScaleYCheckBox->isChecked())
         minMaxGraphValues = getMaxMinValues();

    //    qDebug() << (qint64)minMaxGraphValues.maxValues.x() << (qint64)minMaxGraphValues.minValues.x() << chart->plotArea();

    if(ui->autoScaleXCheckBox->isChecked() && ui->typeComboBox->currentIndex() == PLOT_XY)
        chart->axisX()->setRange( minMaxGraphValues.minValues.x(), minMaxGraphValues.maxValues.x()  );
    else if(ui->typeComboBox->currentIndex() == PLOT_TIME)
        chart->axisX()->setRange( 0, ui->bufferSpin->value()  );
    else
        chart->axisX()->setRange( ui->minXSpin->value(),  ui->maxXSpin->value()  );

    if(ui->autoScaleYCheckBox->isChecked())
        chart->axisY()->setRange( minMaxGraphValues.minValues.y(), minMaxGraphValues.maxValues.y() );
    else
        chart->axisY()->setRange( ui->minYSpin->value(), ui->maxYSpin->value() );

}

line_plot_max_min_values_t LineChart::getMaxMinValues()
{
    line_plot_max_min_values_t result;

    QPointF maxValues;
    QPointF minValues;
    foreach(QString key, items.keys())
    {
        LineChartPlotItem * item = items[key];

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

void LineChart::legendMarkerHovered(bool hovering)
{
    if(hovering)
        this->setCursor(Qt::PointingHandCursor);
    else
        this->setCursor(Qt::ArrowCursor);
}

void LineChart::legendMarkerClicked()
{
    QLegendMarker* marker = qobject_cast<QLegendMarker*>(QObject::sender());
    Q_ASSERT(marker);

    switch(marker->type())
    {
        case QLegendMarker::LegendMarkerTypeXY:
        {
            // Toggle visibility of series
            marker->series()->setVisible(!marker->series()->isVisible());

            // Turn legend marker back to visible, since hiding series also hides the marker
            // and we don't want it to happen now.
            marker->setVisible(true);

            // Dim the marker, if series is not visible
            qreal alpha = 1.0;

            if (!marker->series()->isVisible()) {
                alpha = 0.5;
            }

            QColor color;
            QBrush brush = marker->labelBrush();
            color = brush.color();
            color.setAlphaF(alpha);
            brush.setColor(color);
            marker->setLabelBrush(brush);

            brush = marker->brush();
            color = brush.color();
            color.setAlphaF(alpha);
            brush.setColor(color);
            marker->setBrush(brush);

            QPen pen = marker->pen();
            color = pen.color();
            color.setAlphaF(alpha);
            pen.setColor(color);
            marker->setPen(pen);

            break;
        }
        default: qDebug() << "Unknown marker"; break;
    }
}

void LineChart::xmlStream(QXmlStreamWriter *writer)
{
    writer->writeTextElement("id", this->id);
    writer->writeTextElement("title", ui->titleEdit->text());
    writer->writeTextElement("xname", ui->nameXaxis->text());
    writer->writeTextElement("yname", ui->nameYaxis->text());

    writer->writeTextElement("type", QString::number(ui->typeComboBox->currentIndex()));
    writer->writeTextElement("buffer", QString::number(ui->bufferSpin->value()));
    writer->writeTextElement("refresh_rate", QString::number(ui->refreshRateSpin->value()));

    writer->writeTextElement("animation", QString::number(ui->animationComboBox->currentIndex()) );
    writer->writeTextElement("theme", QString::number(ui->themeComboBox->currentIndex()) );
    writer->writeTextElement("antialiasing", QString::number(ui->antialiasingCheckBox->isChecked()) );

    writer->writeTextElement("legend_enabled", QString::number(ui->legendShowCheckBox->isChecked()) );
    writer->writeTextElement("legend_position", QString::number(ui->legendPositionComboBox->currentIndex()) );
    writer->writeTextElement("legend_hpos", QString::number(ui->legendHPosSpinBox->value()) );
    writer->writeTextElement("legend_vpos", QString::number(ui->legendVPosSpinBox->value()) );
    writer->writeTextElement("legend_width", QString::number(ui->legendWidthSpinBox->value()) );
    writer->writeTextElement("legend_height", QString::number(ui->legendHeightSpinBox->value()) );
    writer->writeTextElement("legend_font", ui->legendFontComboBox->currentText() );
    writer->writeTextElement("legend_bold", QString::number(ui->legendBoldCheckBox->isChecked()) );
    writer->writeTextElement("legend_italic", QString::number(ui->legendItalicCheckBox->isChecked()) );
    writer->writeTextElement("legend_underline", QString::number(ui->legendUnderlineCheckBox->isChecked()) );
    writer->writeTextElement("legend_font_size", QString::number(ui->legendFontSizeSpinBox->value()) );

    writer->writeTextElement("auto_scale_value_axis_x", QString::number(ui->autoScaleXCheckBox->isChecked() ) );
    writer->writeTextElement("min_scale_value_axis_x", QString::number(ui->minXSpin->value()) );
    writer->writeTextElement("max_scale_value_axis_x", QString::number(ui->maxXSpin->value()) );

    writer->writeTextElement("auto_scale_value_axis_y", QString::number(ui->autoScaleYCheckBox->isChecked() ) );
    writer->writeTextElement("min_scale_value_axis_y", QString::number(ui->minYSpin->value()) );
    writer->writeTextElement("max_scale_value_axis_y", QString::number(ui->maxYSpin->value()) );

    writer->writeTextElement("tick_count_x_axis", QString::number(ui->xAxisTickCountSpinBox->value()) );
    writer->writeTextElement("tick_count_y_axis", QString::number(ui->yAxisTickCountSpinBox->value()) );

    writer->writeTextElement( "auto_write_file", ui->writeToFileEdit->text() );
    writer->writeTextElement( "auto_write_seperator", ui->seperatorEdit->text() );
}

void LineChart::xmlParse(QXmlStreamReader *xml)
{
    blockAllSignals(true);
    /* Let's check that we're really getting a person. */
    if(xml->tokenType() != QXmlStreamReader::StartElement &&
            xml->name() == "Widget") {
        return;
    }

    /* Next element... */
    xml->readNext();

    while(!(xml->tokenType() == QXmlStreamReader::EndElement && xml->name() == "Widget"))
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
            chart->setTitle( str );
            ui->titleEdit->setText(str);

            this->setWindowTitle(QString("%1 \t|\t ID: %2").arg(ui->titleEdit->text()).arg(ui->idEdit->text()));
        }

        if(xml->name() == "xname")
        {
            QString str = xml->readElementText();
            ui->nameXaxis->setText(str);
        }

        if(xml->name() == "yname")
        {
            QString str = xml->readElementText();
            ui->nameYaxis->setText(str);
        }

        if(xml->name() == "type")
        {
            int intVal = xml->readElementText().toInt();
            ui->typeComboBox->setCurrentIndex(intVal);
        }

        if(xml->name() == "buffer")
        {
            double realVal = xml->readElementText().toDouble();
            ui->bufferSpin->setValue(realVal);
        }

        if(xml->name() == "refresh_rate")
        {
            double realVal = xml->readElementText().toDouble();
            ui->refreshRateSpin->setValue(realVal);
        }

        if(xml->name() == "animation")
        {
            int intVal = xml->readElementText().toInt();
            ui->animationComboBox->setCurrentIndex(intVal);
        }

        if(xml->name() == "theme")
        {
            int intVal = xml->readElementText().toInt();
            ui->themeComboBox->setCurrentIndex(intVal);
        }

        if(xml->name() == "antialiasing")
        {
            int intVal = xml->readElementText().toInt();
            if(intVal > 0)
                ui->antialiasingCheckBox->setCheckState( Qt::Checked );
            else
                ui->antialiasingCheckBox->setCheckState( Qt::Unchecked );
        }

        if(xml->name() == "legend_enabled")
        {
            int intVal = xml->readElementText().toInt();
            if(intVal > 0)
                ui->legendShowCheckBox->setCheckState( Qt::Checked );
            else
                ui->legendShowCheckBox->setCheckState( Qt::Unchecked );
        }

        if(xml->name() == "legend_position")
        {
            int intVal = xml->readElementText().toInt();
            ui->legendPositionComboBox->setCurrentIndex(intVal);
//            if(intVal == POS_DETACHED)
//                ui->legendDetachedParamGroup->setVisible(true);
        }

        if(xml->name() == "legend_hpos")
        {
            double realVal = xml->readElementText().toDouble();
            ui->legendHPosSpinBox->setValue(realVal);
        }

        if(xml->name() == "legend_vpos")
        {
            double realVal = xml->readElementText().toDouble();
            ui->legendVPosSpinBox->setValue(realVal);
        }

        if(xml->name() == "legend_width")
        {
            double realVal = xml->readElementText().toDouble();
            ui->legendWidthSpinBox->setValue(realVal);
        }

        if(xml->name() == "legend_height")
        {
            double realVal = xml->readElementText().toDouble();
            ui->legendHeightSpinBox->setValue(realVal);
        }

        if(xml->name() == "legend_font")
        {
            QString str = xml->readElementText();
            ui->legendFontComboBox->setCurrentText( str );
        }

        if(xml->name() == "legend_bold")
        {
            int intVal = xml->readElementText().toInt();
            ui->legendBoldCheckBox->setChecked( (bool)intVal );
        }

        if(xml->name() == "legend_italic")
        {
            int intVal = xml->readElementText().toInt();
            ui->legendItalicCheckBox->setChecked( (bool)intVal );
        }

        if(xml->name() == "legend_underline")
        {
            int intVal = xml->readElementText().toInt();
            ui->legendUnderlineCheckBox->setChecked( (bool)intVal );
        }

        if(xml->name() == "legend_font_size")
        {
            double realVal = xml->readElementText().toDouble();
            ui->legendFontSizeSpinBox->setValue(realVal);
        }

        if(xml->name() == "auto_scale_value_axis_x")
        {
            int intVal = xml->readElementText().toInt();
            if(intVal > 0)
                ui->autoScaleXCheckBox->setCheckState( Qt::Checked );
            else
                ui->autoScaleXCheckBox->setCheckState( Qt::Unchecked );
        }

        if(xml->name() == "min_scale_value_axis_x")
        {
            double realVal = xml->readElementText().toDouble();
            ui->minXSpin->setValue(realVal);
        }

        if(xml->name() == "max_scale_value_axis_x")
        {
            double realVal = xml->readElementText().toDouble();
            ui->maxXSpin->setValue(realVal);
        }

        if(xml->name() == "auto_scale_value_axis_y")
        {
            int intVal = xml->readElementText().toInt();
            if(intVal > 0)
                ui->autoScaleYCheckBox->setCheckState( Qt::Checked );
            else
                ui->autoScaleYCheckBox->setCheckState( Qt::Unchecked );
        }

        if(xml->name() == "min_scale_value_axis_y")
        {
            double realVal = xml->readElementText().toDouble();
            ui->minYSpin->setValue(realVal);
        }

        if(xml->name() == "max_scale_value_axis_y")
        {
            double realVal = xml->readElementText().toDouble();
            ui->maxYSpin->setValue(realVal);
        }

        if(xml->name() == "tick_count_x_axis")
        {
            int intVal = xml->readElementText().toInt();
            ui->xAxisTickCountSpinBox->setValue(intVal);
        }

        if(xml->name() == "tick_count_y_axis")
        {
            int intVal = xml->readElementText().toInt();
            ui->yAxisTickCountSpinBox->setValue(intVal);
        }


        if(xml->name() == "auto_write_file")
            ui->writeToFileEdit->setText(  xml->readElementText() );

        if(xml->name() == "auto_write_seperator")
            ui->seperatorEdit->setText(  xml->readElementText() );


        /* ...and next... */
        xml->readNext();
    }

    blockAllSignals(false);
    settingsChanged();
    refreshRateChanged(ui->refreshRateSpin->value());
}

void LineChart::settingsChanged()
{
    qDebug() << "Settings changed by: " << QObject::sender();
    Config::getInstance()->setUnsavedChanges(true);

    axisX->setTitleText( ui->nameXaxis->text() );
    axisY->setTitleText( ui->nameYaxis->text() );

    chart->setAnimationOptions( (QChart::AnimationOption)ui->animationComboBox->currentIndex() );
    ui->chartView->setRenderHint(QPainter::Antialiasing, ui->antialiasingCheckBox->isChecked());

    chart->setTheme( (QChart::ChartTheme) ui->themeComboBox->currentIndex() );
    QFont tFont = chart->titleFont();
    tFont.setPointSizeF( 15 );
    tFont.setBold( true );
    chart->setTitleFont( tFont );


    //Legend settings
    chart->legend()->setVisible(ui->legendShowCheckBox->isChecked());

    //Hmm I haven't figured out yet, how to temprarly hide a set in series... only to delete
    /*foreach(QLegendMarker* marker, chart->legend()->markers())
    {
        disconnect(marker, SIGNAL(clicked()), this, SLOT(legendMarkerClicked()));
        connect(marker, SIGNAL(clicked()), this, SLOT(legendMarkerClicked()));
    }*/

    if(ui->legendPositionComboBox->currentIndex() == POS_DETACHED && chart->legend()->isAttachedToChart() )
    {
        chart->legend()->detachFromChart();
        chart->legend()->setBackgroundVisible(true);
        chart->legend()->setBrush(QBrush(QColor(128, 128, 128, 128)));
        chart->legend()->setPen(QPen(QColor(192, 192, 192, 192)));

        ui->legendDetachedParamGroup->setVisible(true);
    }
    else if(ui->legendPositionComboBox->currentIndex() != POS_DETACHED)
    {
        chart->legend()->attachToChart();
        chart->legend()->setBackgroundVisible(false);
        ui->legendDetachedParamGroup->setVisible(false);
    }

    switch(ui->legendPositionComboBox->currentIndex())
    {
        case POS_TOP: chart->legend()->setAlignment(Qt::AlignTop); break;
        case POS_RIGHT: chart->legend()->setAlignment(Qt::AlignRight); break;
        case POS_BOTTOM: chart->legend()->setAlignment(Qt::AlignBottom); break;
        case POS_LEFT: chart->legend()->setAlignment(Qt::AlignLeft); break;
        case POS_DETACHED: updateDetachedLegendLayout(); break;
        default: chart->legend()->setAlignment(Qt::AlignBottom);
    }
    QFont lFont = ui->legendFontComboBox->currentFont();
    lFont.setBold( ui->legendBoldCheckBox->isChecked() );
    lFont.setItalic( ui->legendItalicCheckBox->isChecked() );
    lFont.setUnderline( ui->legendUnderlineCheckBox->isChecked() );
    lFont.setPointSizeF( ui->legendFontSizeSpinBox->value() );
    chart->legend()->setFont( lFont );

    axisX->setTickCount( ui->xAxisTickCountSpinBox->value() );
    axisY->setTickCount( ui->yAxisTickCountSpinBox->value() );
}

void LineChart::samplingSizeChanged(int value)
{
    foreach(QString key, items.keys())
    {
        items[key]->setSamplingSize( value );
        items[key]->clear();
    }

}

void LineChart::refreshRateChanged(int value)
{
    timer->stop();
    timer->setInterval( value );
    timer->start();
}

void LineChart::updateDetachedLegendLayout()
{
    chart->legend()->setGeometry(QRectF(  ui->legendHPosSpinBox->value(),
                                          ui->legendVPosSpinBox->value(),
                                          ui->legendWidthSpinBox->value(),
                                          ui->legendHeightSpinBox->value()));
    chart->legend()->update();
}

void LineChart::setId(QString str)
{
   id = str;
   this->setWindowTitle(QString("%1 \t|\t ID: %2").arg(ui->titleEdit->text()).arg(str));

   ui->idEdit->blockSignals(true);
   ui->idEdit->setText( str );
   ui->idEdit->blockSignals(false);

   Config::getInstance()->setUnsavedChanges(true);

}

void LineChart::setTitle(QString str)
{
    chart->setTitle( str );
    this->setWindowTitle(QString("%1 \t|\t ID: %2").arg(str).arg(id));
    Config::getInstance()->setUnsavedChanges(true);
}

void LineChart::setFileForWriting()
{
    QString fileName = QFileDialog::getSaveFileName(
                    this,
                    "Save Plot to CSV",
                    QString(),
                    "Comma Seperated Values (*.csv);;All Files (*)"
    );

    ui->writeToFileEdit->setText( fileName );


}

void LineChart::startStopAutoWriting(bool start)
{
    if(ui->writeToFileEdit->text().isEmpty())
    {
        ui->writeToFileCheckBox->setChecked(false);
        return;
    }

    if(start == true)
    {
        if(autoWriteFile != NULL)
            delete autoWriteFile;

        autoWriteFile = new QFile(ui->writeToFileEdit->text());

        if(!autoWriteFile->open(QIODevice::WriteOnly))
        {
            qCritical() << "Error couldn't open file for writing";
            return;
        }

        autoWriteStream.setDevice(autoWriteFile);

        autoWriteStream.setRealNumberNotation( QTextStream::SmartNotation );

        bool first = true;
        foreach(QString key, items.keys())
        {
            if(first)
                first = false;
            else
                autoWriteStream << ui->seperatorEdit->text();

            autoWriteStream << key << "_x" << ui->seperatorEdit->text() << key << "_y";
        }

        autoWriteStream << endl;
    }
    else
    {
        if(autoWriteFile != NULL)
            autoWriteFile->close();
    }
}

void LineChart::savePlot()
{
    QString fileName = QFileDialog::getSaveFileName(
                    this,
                    "Save Plot to CSV",
                    QString(),
                    "Comma Seperated Values (*.csv);;All Files (*)"
    );

    QFile file(fileName);
    if(!file.open(QIODevice::WriteOnly))
    {
        qCritical() << "Error couldn't open file for writing";
        return;
    }

    QTextStream stream(&file);
    stream.setRealNumberNotation( QTextStream::SmartNotation );

    bool first = true;
    foreach(QString key, items.keys())
    {
        if(first)
            first = false;
        else
            stream << ui->seperatorEdit->text();

        stream << key << "_x" << ui->seperatorEdit->text() << key << "_y";
    }

    stream << endl;


    for (int i=0; i<items[items.keys().first()]->getData().size(); ++i)
    {
        first = true;
        foreach(QString key, items.keys())
        {
            if(first)
                first = false;
            else
                stream << ui->seperatorEdit->text();


            if( items[key]->getData().size() > i)
            {
                stream << items[key]->getData().at(i).x() << ui->seperatorEdit->text() << items[key]->getData().at(i).y();
            }
            else
                stream << "0" << ui->seperatorEdit->text() << "0";
        }

        stream << endl;
    }

    file.close();
}

void LineChart::startStopPlotting()
{
    if(ui->startStopPlotBtn->isChecked())
    {
        doPlotting = true;
        timer->start();
        plot();

        startStopAutoWriting(ui->writeToFileCheckBox->isChecked());
    }
    else
    {
        doPlotting = false;
        timer->stop();

        if(autoWriteFile != NULL && autoWriteFile->isOpen() && ui->writeToFileCheckBox->isChecked())
        {
            autoWriteFile->close();
            delete autoWriteFile;
            autoWriteFile = NULL;
        }
    }
}

void LineChart::blockAllSignals(bool b)
{
    ui->idEdit->blockSignals(b);
    ui->titleEdit->blockSignals(b);
    ui->nameXaxis->blockSignals(b);
    ui->nameYaxis->blockSignals(b);

    ui->animationComboBox->blockSignals(b);
    ui->themeComboBox->blockSignals(b);
    ui->antialiasingCheckBox->blockSignals(b);

    ui->legendShowCheckBox->blockSignals(b);
    ui->legendBoldCheckBox->blockSignals(b);
    ui->legendItalicCheckBox->blockSignals(b);
    ui->legendUnderlineCheckBox->blockSignals(b);
    ui->legendFontSizeSpinBox->blockSignals(b);
    ui->legendFontComboBox->blockSignals(b);
    ui->legendPositionComboBox->blockSignals(b);
    ui->legendHPosSpinBox->blockSignals(b);
    ui->legendVPosSpinBox->blockSignals(b);
    ui->legendWidthSpinBox->blockSignals(b);
    ui->legendHeightSpinBox->blockSignals(b);

    ui->yAxisTickCountSpinBox->blockSignals(b);
    ui->xAxisTickCountSpinBox->blockSignals(b);
}

LineChart::~LineChart()
{
    delete ui;
}



//--------------------------------------------------------------------
// LineChartPlotItem object
//--------------------------------------------------------------------

LineChartPlotItem::LineChartPlotItem(QXYSeries *m_series, QString name)
{
    this->m_series = m_series;
    this->m_series->setName( name );
    colorIsSet = false;
    this->name = name;

    m_samplingSize = 500;
}

void LineChartPlotItem::update()
{
    m_series->replace( points );
}

void LineChartPlotItem::addData(QPointF p)
{
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
            //sine RT line chart can't have nice timestamp, there is no reason not to alter X with sample index
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


}

void LineChartPlotItem::setColor(QColor color)
{
    m_series->setColor( color );
    colorIsSet = true;
    this->color = color;
}

void LineChartPlotItem::deleteFirst()
{
    points.takeFirst();
}

void LineChartPlotItem::clear()
{
    points.clear();
}

QPointF LineChartPlotItem::maxValues()
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

QPointF LineChartPlotItem::minValues()
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

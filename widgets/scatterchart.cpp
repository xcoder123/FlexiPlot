#include "scatterchart.h"
#include "ui_scatterchart.h"

ScatterChart::ScatterChart(QWidget *parent) :
    AbstractWidget(parent),
    ui(new Ui::ScatterChart)
{
    ui->setupUi(this);

    doPlotting = true;

    chart = new QChart();


//    chart->setDropShadowEnabled(true);

    chart->setTitle( ui->titleEdit->text() );
    QFont tFont = chart->titleFont();
    tFont.setPointSizeF( 15 );
    tFont.setBold( true );
    chart->setTitleFont( tFont );

    chart->setAnimationOptions(QChart::NoAnimation);



    QValueAxis * axisX = new QValueAxis();
    QValueAxis * axisY = new QValueAxis();
    chart->setAxisX( axisX );
    chart->setAxisY( axisY );

    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);

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

    connect(ui->markerComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(markerPropertiesChanged()));
    connect(ui->markerSizeSpinBox, SIGNAL(valueChanged(int)), this, SLOT(markerPropertiesChanged()));
    connect(ui->bufferSpin, SIGNAL(valueChanged(int)), this, SLOT(samplingSizeChanged(int)));

    connect(ui->animationComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(settingsChanged()));
    connect(ui->themeComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(settingsChanged()));
    connect(ui->antialiasingCheckBox, SIGNAL(stateChanged(int)), this, SLOT(settingsChanged()));
    connect(ui->useOpenGLCheckBox, SIGNAL(stateChanged(int)), this, SLOT(openGlStateChanged()));

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

    connect(ui->xAxisTickCountSpinBox, SIGNAL(valueChanged(int)), this, SLOT(settingsChanged()));
    connect(ui->yAxisTickCountSpinBox, SIGNAL(valueChanged(int)), this, SLOT(settingsChanged()));


}

bool ScatterChart::validPacket(QString packet)
{
   /*
    * Regular expression for XY scatter plot
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

    QRegExp fullPacket("[a-zA-Z0-9]+(\\|[a-zA-Z0-9 ]+\\|\\d{1,3},\\d{1,3},\\d{1,3}\\|(\\-*\\d+(\\.{0,1}\\d+)*\\s?)*)+");
    QRegExp noColorPacket("[a-zA-Z0-9]+(\\|[a-zA-Z0-9 ]+\\|(\\-*\\d+(\\.{0,1}\\d+)*\\s?)*)+");

//    qDebug() << fullPacket.exactMatch(packet) << noColorPacket.exactMatch(packet);
    return fullPacket.exactMatch(packet) || noColorPacket.exactMatch(packet);
}

void ScatterChart::reset()
{
    foreach(QString key, items.keys())
        delete items.value(key);
    items.clear();

    chart->removeAllSeries();
}

void ScatterChart::clear()
{
    reset();
}

void ScatterChart::serialPacket(QStringList packet)
{
    if(doPlotting == false)
        return;

    for(int i=1; i<packet.length(); i++)
    {
        QScatterSeries * series = NULL;
        if(!items.keys().contains(packet[i])) //does it have the current item?
        {
           series =  new QScatterSeries();
           series->setMarkerShape((QScatterSeries::MarkerShape)ui->markerComboBox->currentIndex());
           series->setMarkerSize(ui->markerSizeSpinBox->value());
           series->setUseOpenGL(ui->useOpenGLCheckBox->isChecked());
           chart->addSeries( series );
           series->attachAxis( chart->axisX() );
           series->attachAxis( chart->axisY() );

           /*foreach(QLegendMarker* marker, chart->legend()->markers())
           {
               disconnect(marker, SIGNAL(clicked()), this, SLOT(legendMarkerClicked()));
               disconnect(marker, SIGNAL(hovered(bool)), this, SLOT(legendMarkerHovered(bool)));

               connect(marker, SIGNAL(clicked()), this, SLOT(legendMarkerClicked()));
               connect(marker, SIGNAL(hovered(bool)), this, SLOT(legendMarkerHovered(bool)));
           }*/


           XYChartPlotItem* plotItem = new XYChartPlotItem(series, packet[i], XYChartPlotItem::UPDATE_INSTANTLY);


           plotItem->setSamplingSize(ui->bufferSpin->value());
           items.insert(packet[i], plotItem);
        }

        XYChartPlotItem* plotItem = items[packet[i]];


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

        if(values.length() % 2 == 0)
        {
            for(int vI=0; vI<values.length(); vI += 2)
            {
                plotItem->addData( QPointF( values.at(vI).toDouble(), values.at(vI+1).toDouble() ) );
//                qDebug() << QPointF( values.at(vI).toDouble(), values.at(vI+1).toDouble() );
            }
        }
        else
        {
            //Invalid packet, missing pairs.
            qDebug() << "Invalid packet, missing pairs.";
        }

    }

    plot();
}

void ScatterChart::plot()
{
    foreach(QString key, items.keys())
    {
//        qDebug() << key << items[key]->series()->useOpenGL() << items[key]->getData();
        items[key]->update();
//        qDebug()<< qobject_cast<QScatterSeries*>(items[key]->series())->markerShape();
    }


    xy_items_max_min_values_t minMaxGraphValues;
    if(ui->autoScaleXCheckBox->isChecked() || ui->autoScaleYCheckBox->isChecked())
         minMaxGraphValues = XYChartPlotItem::getMaxMinValues( items );

    //    qDebug() << (qint64)minMaxGraphValues.maxValues.x() << (qint64)minMaxGraphValues.minValues.x() << chart->plotArea();

   if(ui->autoScaleXCheckBox->isChecked())
        chart->axisX()->setRange( minMaxGraphValues.minValues.x(), minMaxGraphValues.maxValues.x()  );
    else
        chart->axisX()->setRange( ui->minXSpin->value(),  ui->maxXSpin->value()  );

    if(ui->autoScaleYCheckBox->isChecked())
        chart->axisY()->setRange( minMaxGraphValues.minValues.y(), minMaxGraphValues.maxValues.y() );
    else
        chart->axisY()->setRange( ui->minYSpin->value(), ui->maxYSpin->value() );

}

void ScatterChart::xmlStream(QXmlStreamWriter *writer)
{
    writer->writeTextElement("id", this->id);
    writer->writeTextElement("title", ui->titleEdit->text());
    writer->writeTextElement("xname", ui->nameXaxis->text());
    writer->writeTextElement("yname", ui->nameYaxis->text());

    writer->writeTextElement("marker_shape", QString::number(ui->markerComboBox->currentIndex()));
    writer->writeTextElement("marker_size", QString::number(ui->markerSizeSpinBox->value()));
    writer->writeTextElement("buffer", QString::number(ui->bufferSpin->value()));

    writer->writeTextElement("use_opengl", QString::number(ui->useOpenGLCheckBox->isChecked()) );
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

    writer->writeTextElement( "auto_write_seperator", ui->seperatorEdit->text() );
}

void ScatterChart::xmlParse(QXmlStreamReader *xml)
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

        if(xml->name() == "marker_shape")
        {
            int intVal = xml->readElementText().toInt();
            ui->markerComboBox->setCurrentIndex( (QScatterSeries::MarkerShape)intVal );
        }

        if(xml->name() == "marker_size")
        {
            int intVal = xml->readElementText().toInt();
            ui->markerSizeSpinBox->setValue( intVal );
        }

        if(xml->name() == "buffer")
        {
            double realVal = xml->readElementText().toDouble();
            ui->bufferSpin->setValue(realVal);
        }

        if(xml->name() == "use_opengl")
        {
            int intVal = xml->readElementText().toInt();
            if(intVal > 0)
                ui->useOpenGLCheckBox->setCheckState( Qt::Checked );
            else
                ui->useOpenGLCheckBox->setCheckState( Qt::Unchecked );
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


        if(xml->name() == "auto_write_seperator")
            ui->seperatorEdit->setText(  xml->readElementText() );


        /* ...and next... */
        xml->readNext();
    }

    blockAllSignals(false);

    //openGlStateChanged() should also invoke: checkIfAnimationsAllowed and settingsChanged
    openGlStateChanged();
//    checkIfAnimationsAllowed();
//    settingsChanged();
}

void ScatterChart::settingsChanged()
{
    qDebug() << "Settings changed by: " << QObject::sender();
    Config::getInstance()->setUnsavedChanges(true);

    chart->axisX()->setTitleText( ui->nameXaxis->text() );
    chart->axisY()->setTitleText( ui->nameYaxis->text() );


    chart->setAnimationOptions( (QChart::AnimationOption)ui->animationComboBox->currentIndex() );
    ui->chartView->setRenderHint(QPainter::Antialiasing, ui->antialiasingCheckBox->isChecked());

    chart->setTheme( (QChart::ChartTheme) ui->themeComboBox->currentIndex() );
    QFont tFont = chart->titleFont();
    tFont.setPointSizeF( 15 );
    tFont.setBold( true );
    chart->setTitleFont( tFont );


    //Legend settings
    chart->legend()->setVisible(ui->legendShowCheckBox->isChecked());

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

    QValueAxis * axisX = qobject_cast<QValueAxis*>( chart->axisX() );
    axisX->setTickCount( ui->xAxisTickCountSpinBox->value() );
    QValueAxis * axisY = qobject_cast<QValueAxis*>( chart->axisY() );
    axisY->setTickCount( ui->yAxisTickCountSpinBox->value() );
}

void ScatterChart::updateDetachedLegendLayout()
{
    chart->legend()->setGeometry(QRectF(  ui->legendHPosSpinBox->value(),
                                          ui->legendVPosSpinBox->value(),
                                          ui->legendWidthSpinBox->value(),
                                          ui->legendHeightSpinBox->value()));
    chart->legend()->update();
}

void ScatterChart::samplingSizeChanged(int value)
{
    foreach(QString key, items.keys())
    {
        items[key]->setSamplingSize( value );
        items[key]->clear();
    }

    checkIfAnimationsAllowed();
    settingsChanged();
}

void ScatterChart::openGlStateChanged()
{
    ui->markerComboBox->blockSignals(true);
    if(ui->useOpenGLCheckBox->isChecked())
    {
        ui->markerComboBox->setCurrentIndex( QScatterSeries::MarkerShapeRectangle );
        ui->markerComboBox->setEnabled(false);
    }
    else
    {
        ui->markerComboBox->setEnabled(true);
    }
    ui->markerComboBox->blockSignals(false);

    checkIfAnimationsAllowed();

    foreach(XYChartPlotItem* item, items)
    {
        QScatterSeries* series = qobject_cast<QScatterSeries*>(item->series());
        series->setUseOpenGL( ui->useOpenGLCheckBox->isChecked() );
    }

    settingsChanged();
}

void ScatterChart::checkIfAnimationsAllowed()
{
    ui->animationComboBox->blockSignals(true);

    if(ui->bufferSpin->value() > 0 || ui->useOpenGLCheckBox->isChecked() )
    {
        ui->animationComboBox->setCurrentIndex( QChart::NoAnimation );
        ui->animationComboBox->setEnabled(false);
    }
    else
    {
        ui->animationComboBox->setEnabled(true);
    }

    ui->animationComboBox->blockSignals(false);
}

void ScatterChart::markerPropertiesChanged()
{
    foreach(XYChartPlotItem* item, items)
    {
        QScatterSeries* series = qobject_cast<QScatterSeries*>(item->series());
        series->setMarkerShape( (QScatterSeries::MarkerShape)ui->markerComboBox->currentIndex() );
        series->setMarkerSize( ui->markerSizeSpinBox->value() );
    }
}

void ScatterChart::setId(QString str)
{
    id = str;

    this->setWindowTitle(QString("%1 \t|\t ID: %2").arg(ui->titleEdit->text()).arg(str));

    ui->idEdit->blockSignals(true);
    ui->idEdit->setText( str );
    ui->idEdit->blockSignals(false);

    Config::getInstance()->setUnsavedChanges(true);
}

void ScatterChart::setTitle(QString str)
{
    chart->setTitle( str );
    this->setWindowTitle(QString("%1 \t|\t ID: %2").arg(str).arg(id));
    Config::getInstance()->setUnsavedChanges(true);
}

void ScatterChart::savePlot()
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

void ScatterChart::startStopPlotting()
{
    if(ui->startStopPlotBtn->isChecked())
    {
        doPlotting = true;
        plot();

    }
    else
    {
        doPlotting = false;
    }
}

void ScatterChart::blockAllSignals(bool b)
{
    ui->idEdit->blockSignals(b);
    ui->titleEdit->blockSignals(b);
    ui->nameXaxis->blockSignals(b);
    ui->nameYaxis->blockSignals(b);

    ui->markerComboBox->blockSignals(b);
    ui->markerSizeSpinBox->blockSignals(b);

    ui->useOpenGLCheckBox->blockSignals(b);
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

ScatterChart::~ScatterChart()
{
    delete ui;
}

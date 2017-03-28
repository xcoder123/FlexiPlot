#include "bargraph.h"
#include "ui_bargraph.h"

QT_CHARTS_USE_NAMESPACE

BarGraph::BarGraph(QWidget *parent) :
    AbstractWidget(parent),
    ui(new Ui::BarGraph)
{
    ui->setupUi(this);




    chart = new QChart();


    chart->setTitle( ui->titleEdit->text() );
    QFont tFont = chart->titleFont();
    tFont.setPointSizeF( 15 );
    tFont.setBold( true );
    chart->setTitleFont( tFont );

    chart->setAnimationOptions(QChart::SeriesAnimations);

    chart->createDefaultAxes();

    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);

//    QChartView *chartView = new QChartView(chart);
    ui->chartView->setChart( chart );
    ui->chartView->setRenderHint(QPainter::Antialiasing);

//    this->layout()->addWidget( chartView );

    this->setWindowTitle(ui->titleEdit->text());

    connect(ui->idEdit, SIGNAL(textEdited(QString)), this, SLOT(setId(QString)));
    connect(ui->titleEdit, SIGNAL(textEdited(QString)), this, SLOT(setTitle(QString)));
    connect(ui->nameXaxis, SIGNAL(textEdited(QString)), this, SLOT(settingsChanged()));
    connect(ui->nameYaxis, SIGNAL(textEdited(QString)), this, SLOT(settingsChanged()));

    connect(ui->barChartTypeComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(settingsChanged()));
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

    connect(ui->showLabelsCheckBox, SIGNAL(stateChanged(int)), this, SLOT(settingsChanged()));
    connect(ui->labelAngleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(settingsChanged()));
    connect(ui->labelFormatEdit, SIGNAL(textChanged(QString)), this, SLOT(settingsChanged()));
    connect(ui->labelPosComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(settingsChanged()));

    connect(ui->autoScaleValueAxisCheckBox, SIGNAL(stateChanged(int)), this, SLOT(settingsChanged()));
    connect(ui->maxValueAxisSpinBox, SIGNAL(valueChanged(double)), this, SLOT(settingsChanged()));
    connect(ui->minValueAxisSpinBox, SIGNAL(valueChanged(double)), this, SLOT(settingsChanged()));


    readyToPlot = true;
}

void BarGraph::settingsChanged()
{
    if(!readyToPlot)
        return;

    qDebug() << "Settings changed by: " << QObject::sender();
    Config::getInstance()->setUnsavedChanges(true);
    plot();
}

void BarGraph::setTitle(QString str)
{
    chart->setTitle( str );
    this->setWindowTitle(QString("%1 \t|\t ID: %2").arg(str).arg(id));
    Config::getInstance()->setUnsavedChanges(true);
}

bool BarGraph::validPacket(QString packet)
{
    /* Regular expresion for multi column packet:
     * Colo Test cases:
     *             P0|Jan;Feb;Mar|Jane|255,0,0|25 15 17|John|255,0,0|8 9 23
     *             P0|Jan|Jane|255,0,0|25|John|255,0,0|8
     * Regexp:
     *             [a-zA-Z0-9]+\|[a-zA-Z0-9 \;]+(\|[a-zA-Z0-9 ]+\|\d{1,3},\d{1,3},\d{1,3}\|(\-*\d+(\.{0,1}\d+)*\s?)*)+
     *
     *
     * No Color Test Cases:
     *              P0|Jan;Feb;Mar|Jane|25 15 17|John|8 9 23
     *              P0|Jan|Jane|25|John|8
     *
     * Regexp:
     *              [a-zA-Z0-9]+\|[a-zA-Z0-9 \;]+(\|[a-zA-Z0-9 ]+\|(\-*\d+(\.{0,1}\d+)*\s?)*)+
     *
     */
    QRegExp fullPacket("[a-zA-Z0-9]+\\|[a-zA-Z0-9 \\;]+(\\|[a-zA-Z0-9 ]+\\|\\d{1,3},\\d{1,3},\\d{1,3}\\|(\\-*\\d+(\\.{0,1}\\d+)*\\s?)*)+");
    QRegExp noColorPacket("[a-zA-Z0-9]+\\|[a-zA-Z0-9 \\;]+(\\|[a-zA-Z0-9 ]+\\|(\\-*\\d+(\\.{0,1}\\d+)*\\s?)*)+");

//    qDebug() << fullPacket.exactMatch(packet) << noColorPacket.exactMatch( packet );

    return fullPacket.exactMatch(packet) || noColorPacket.exactMatch( packet );
}

void BarGraph::serialPacket(QStringList packet)
{
//    qDebug() << packet;
    categories = packet[1].split(";");

//    qDebug() << categories;

    items.clear();

    for(int i=2; i<packet.length(); i++)
    {
        bar_graph_plot_item_t item;

        item.set_name = packet[i];

        QRegExp rx("\\d{1,},\\d{1,},\\d{1,}");
        QRegExpValidator v(rx,this);

        QStringList values;
        if(packet[i+1].contains(rx)) //This set has a color specified
        {
            QStringList color = packet[i+1].split(",");
            item.color = QColor(color[0].toInt(),color[1].toInt(),color[2].toInt());
            item.colorIsSet = true;

            values = packet[i+2].split(" ");
            i += 2;
        }
        else
        {
            values = packet[i+1].split(" ");
            i += 1;
        }

        foreach(QString valueStr, values)
            item.values << valueStr.toDouble();

        items.append( item );

    }

    plot();
}

void BarGraph::plot()
{
    if(!readyToPlot)
        return;

    qDeleteAll( chart->series() );
    qDeleteAll( chart->axes() );

    chart->setAnimationOptions( (QChart::AnimationOption)ui->animationComboBox->currentIndex() );
    ui->chartView->setRenderHint(QPainter::Antialiasing, ui->antialiasingCheckBox->isChecked());

    QAbstractBarSeries* series;
    switch( ui->barChartTypeComboBox->currentIndex() ) {
        case TYPE_NORMAL: series = new QBarSeries(); break;
        case TYPE_STACKED: series = new QStackedBarSeries(); break;
        case TYPE_PERCENT: series = new QPercentBarSeries(); break;
        case TYPE_HORIZONTAL: series = new QHorizontalBarSeries(); break;
        case TYPE_HORIZONTAL_STACKED: series = new QHorizontalStackedBarSeries(); break;
        case TYPE_HORIZONTAL_PERCENT: series = new QHorizontalPercentBarSeries(); break;
        default: series = new QBarSeries();
    }

//    series->setLabelsVisible( true );

    foreach(bar_graph_plot_item_t item, items)
    {
        QBarSet *set = new QBarSet(item.set_name);
        if(item.colorIsSet)
            set->setColor( item.color );
        set->append( item.values );

        series->append( set );
    }

    chart->addSeries(series);

    QBarCategoryAxis *axis = new QBarCategoryAxis();
    axis->append(categories);


    chart->createDefaultAxes();
    if(     ui->barChartTypeComboBox->currentIndex() == TYPE_NORMAL ||
            ui->barChartTypeComboBox->currentIndex() == TYPE_STACKED ||
            ui->barChartTypeComboBox->currentIndex() == TYPE_PERCENT)
    {
        chart->setAxisX(axis, series);

        if(!ui->autoScaleValueAxisCheckBox->isChecked())
        {
            QAbstractAxis *aValueAxis = chart->axes(Qt::Vertical).at(0);
            QValueAxis  *valueAxis = qobject_cast<QValueAxis *>( aValueAxis );
//            qDebug() << chart->axes(Qt::Vertical).at(0) << ay << ay->max();
            valueAxis->setMax( ui->maxValueAxisSpinBox->value() );
            valueAxis->setMin( ui->minValueAxisSpinBox->value() );
        }
    }
    else
    {
        chart->setAxisY(axis, series);

        if(!ui->autoScaleValueAxisCheckBox->isChecked())
        {
            QAbstractAxis *aValueAxis = chart->axes(Qt::Horizontal).at(0);
            QValueAxis  *valueAxis = qobject_cast<QValueAxis *>( aValueAxis );
//            qDebug() << chart->axes(Qt::Vertical).at(0) << ay << ay->max();
            valueAxis->setMax( ui->maxValueAxisSpinBox->value() );
            valueAxis->setMin( ui->minValueAxisSpinBox->value() );
        }
    }

    chart->axisX()->setTitleText( ui->nameXaxis->text() );
    chart->axisY()->setTitleText( ui->nameYaxis->text() );




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


    //Label config
    series->setLabelsVisible( ui->showLabelsCheckBox->isChecked() );
    if(ui->showLabelsCheckBox->isChecked())
    {
        series->setLabelsAngle( ui->labelAngleSpinBox->value() );
        series->setLabelsFormat( ui->labelFormatEdit->text() );
        series->setLabelsPosition( (QAbstractBarSeries::LabelsPosition)ui->labelPosComboBox->currentIndex() );
    }


    chart->setTheme( (QChart::ChartTheme) ui->themeComboBox->currentIndex() );
    QFont tFont = chart->titleFont();
    tFont.setPointSizeF( 15 );
    tFont.setBold( true );
    chart->setTitleFont( tFont );




}

void BarGraph::updateDetachedLegendLayout()
{
    chart->legend()->setGeometry(QRectF(  ui->legendHPosSpinBox->value(),
                                          ui->legendVPosSpinBox->value(),
                                          ui->legendWidthSpinBox->value(),
                                          ui->legendHeightSpinBox->value()));
    chart->legend()->update();
}

void BarGraph::xmlStream(QXmlStreamWriter *writer)
{
    writer->writeTextElement("id", this->id);
    writer->writeTextElement("title", ui->titleEdit->text());
    writer->writeTextElement("xname", ui->nameXaxis->text());
    writer->writeTextElement("yname", ui->nameYaxis->text());

    writer->writeTextElement("type", QString::number(ui->barChartTypeComboBox->currentIndex()) );
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

    writer->writeTextElement("labels_enabled", QString::number(ui->showLabelsCheckBox->isChecked()) );
    writer->writeTextElement("labels_angle", QString::number(ui->labelAngleSpinBox->value()) );
    writer->writeTextElement("labels_position", QString::number(ui->labelPosComboBox->currentIndex()) );
    writer->writeTextElement("labels_format", ui->labelFormatEdit->text() );

    writer->writeTextElement("auto_scale_value_axis", QString::number(ui->autoScaleValueAxisCheckBox->isChecked() ) );
    writer->writeTextElement("min_scale_value_axis", QString::number(ui->minValueAxisSpinBox->value()) );
    writer->writeTextElement("max_scale_value_axis", QString::number(ui->maxValueAxisSpinBox->value()) );


}

void BarGraph::xmlParse(QXmlStreamReader *xml)
{
    readyToPlot = false;
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
            ui->barChartTypeComboBox->setCurrentIndex(intVal);
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

        if(xml->name() == "labels_enabled")
        {
            int intVal = xml->readElementText().toInt();
            if(intVal > 0)
                ui->showLabelsCheckBox->setCheckState( Qt::Checked );
            else
                ui->showLabelsCheckBox->setCheckState( Qt::Unchecked );
        }

        if(xml->name() == "labels_angle")
        {
            double realVal = xml->readElementText().toDouble();
            ui->labelAngleSpinBox->setValue(realVal);
        }

        if(xml->name() == "labels_position")
        {
            int intVal = xml->readElementText().toInt();
            ui->labelPosComboBox->setCurrentIndex(intVal);
        }

        if(xml->name() == "labels_format")
        {
            QString str = xml->readElementText();
            ui->labelFormatEdit->setText( str );
        }

        if(xml->name() == "auto_scale_value_axis")
        {
            int intVal = xml->readElementText().toInt();
            if(intVal > 0)
                ui->autoScaleValueAxisCheckBox->setCheckState( Qt::Checked );
            else
                ui->autoScaleValueAxisCheckBox->setCheckState( Qt::Unchecked );
        }

        if(xml->name() == "min_scale_value_axis")
        {
            double realVal = xml->readElementText().toDouble();
            ui->minValueAxisSpinBox->setValue(realVal);
        }

        if(xml->name() == "max_scale_value_axis")
        {
            double realVal = xml->readElementText().toDouble();
            ui->maxValueAxisSpinBox->setValue(realVal);
        }


        /* ...and next... */
        xml->readNext();
    }

    readyToPlot = true;
    blockAllSignals(false);
    settingsChanged();
}

void BarGraph::reset()
{
    qDeleteAll( chart->series() );
    qDeleteAll( chart->axes() );
}

void BarGraph::setId(QString str)
{
   id = str;
   this->setWindowTitle(QString("%1 \t|\t ID: %2").arg(ui->titleEdit->text()).arg(str));

   ui->idEdit->blockSignals(true);
   ui->idEdit->setText( str );
   ui->idEdit->blockSignals(false);

   Config::getInstance()->setUnsavedChanges(true);

}

//Hmm I haven't figured out yet, how to temprarly hide a set in series... only to delete
void BarGraph::legendMarkerClicked()
{
    QLegendMarker* marker = qobject_cast<QLegendMarker*>(QObject::sender());
    Q_ASSERT(marker);

    qDebug() << marker->type();

    switch(marker->type())
    {
        case QLegendMarker::LegendMarkerTypeBar:
        {
            QAbstractBarSeries * barSeries = qobject_cast<QAbstractBarSeries*>( marker->series() );
            Q_ASSERT(barSeries);


            qDebug() << barSeries->barSets().at(0);

            QBarLegendMarker* barMarker = qobject_cast<QBarLegendMarker*>(marker);
            Q_ASSERT(barMarker);
            qDebug() << barMarker->barset();

            barSeries->take(barMarker->barset());


            break;
        }
        default: qDebug() << "Unknown marker"; break;
    }
}

void BarGraph::blockAllSignals(bool block)
{
    ui->idEdit->blockSignals(block);
    ui->titleEdit->blockSignals(block);
    ui->nameXaxis->blockSignals(block);
    ui->nameYaxis->blockSignals(block);

    ui->barChartTypeComboBox->blockSignals(block);
    ui->animationComboBox->blockSignals(block);
    ui->themeComboBox->blockSignals(block);

    ui->legendShowCheckBox->blockSignals(block);
    ui->legendBoldCheckBox->blockSignals(block);
    ui->legendItalicCheckBox->blockSignals(block);
    ui->legendUnderlineCheckBox->blockSignals(block);
    ui->legendFontSizeSpinBox->blockSignals(block);
    ui->legendFontComboBox->blockSignals(block);
    ui->legendPositionComboBox->blockSignals(block);
    ui->legendHPosSpinBox->blockSignals(block);
    ui->legendVPosSpinBox->blockSignals(block);
    ui->legendWidthSpinBox->blockSignals(block);
    ui->legendHeightSpinBox->blockSignals(block);

    ui->showLabelsCheckBox->blockSignals(block);
    ui->labelAngleSpinBox->blockSignals(block);
    ui->labelFormatEdit->blockSignals(block);
    ui->labelPosComboBox->blockSignals(block);

    ui->autoScaleValueAxisCheckBox->blockSignals(block);
    ui->minValueAxisSpinBox->blockSignals(block);
    ui->maxValueAxisSpinBox->blockSignals(block);
}

BarGraph::~BarGraph()
{
    delete ui;
}

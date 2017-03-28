#include "piechart.h"
#include "ui_piechart.h"

QT_CHARTS_USE_NAMESPACE

PieChart::PieChart(QWidget *parent) :
    AbstractWidget(parent),
    ui(new Ui::PieChart)
{
    ui->setupUi(this);

    chart = new QChart();

    chart->setTitle( ui->titleEdit->text() );
    QFont tFont = chart->titleFont();
    tFont.setPointSizeF( 15 );
    tFont.setBold( true );
    chart->setTitleFont( tFont );


    chart->setAnimationOptions( QChart::AllAnimations );

    ui->chartView->setChart( chart );
    ui->chartView->setRenderHint(QPainter::Antialiasing);


    connect(ui->idEdit, SIGNAL(textEdited(QString)), this, SLOT(setId(QString)));
    connect(ui->titleEdit, SIGNAL(textEdited(QString)), this, SLOT(setTitle(QString)));

    connect(ui->hPosSpinBox, SIGNAL(valueChanged(double)), this, SLOT(settingsChanged()));
    connect(ui->vPosSpinBox, SIGNAL(valueChanged(double)), this, SLOT(settingsChanged()));
    connect(ui->sizeFactorSpinBox, SIGNAL(valueChanged(double)), this, SLOT(settingsChanged()));
    connect(ui->startAngleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(settingsChanged()));
    connect(ui->endAngleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(settingsChanged()));
    connect(ui->holeSizeSpinBox, SIGNAL(valueChanged(double)), this, SLOT(settingsChanged()));
    connect(ui->explodeSliceCheckBox, SIGNAL(stateChanged(int)), this, SLOT(settingsChanged()));
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

    connect(ui->labelArmLengthSpinBox, SIGNAL(valueChanged(double)), this, SLOT(settingsChanged()));
    connect(ui->labelPosComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(settingsChanged()));
    connect(ui->showLabelsCheckBox, SIGNAL(stateChanged(int)), this, SLOT(settingsChanged()));

}

void PieChart::settingsChanged()
{
    qDebug() << "Settings changed by: " << QObject::sender();
    Config::getInstance()->setUnsavedChanges(true);
    plot();
}

bool PieChart::validPacket(QString packet)
{
    /* Regular expresion for multi column packet:
     * Colo Test cases:
     *             P0|Slice 1|255,0,0|10.0048|Slice 2|0,255,0|30|Slice 3|0,0,255|20
     *
     * Regexp:
     *             [a-zA-Z0-9]+(\|[a-zA-Z0-9 ]+\|\d{1,3},\d{1,3},\d{1,3}\|(\-*\d+(\.{0,1}\d+)*)*)+
     *
     *
     * No Color Test Cases:
     *              P0|Slice 1|10|Slice 2|30|Slice 3|20
     *
     * Regexp:
     *              [a-zA-Z0-9]+(\|[a-zA-Z0-9 ]+\|(\-*\d+(\.{0,1}\d+)*)*)+
     *
     */

    QRegExp fullPacket("[a-zA-Z0-9]+(\\|[a-zA-Z0-9 ]+\\|\\d{1,3},\\d{1,3},\\d{1,3}\\|(\\-*\\d+(\\.{0,1}\\d+)*)*)+");
    QRegExp noColorPacket("[a-zA-Z0-9]+(\\|[a-zA-Z0-9 ]+\\|(\\-*\\d+(\\.{0,1}\\d+)*)*)+");


    return fullPacket.exactMatch(packet) || noColorPacket.exactMatch( packet );
}

void PieChart::reset()
{

}

void PieChart::serialPacket(QStringList packet)
{
//    qDebug() << packet;

    items.clear();

    for(int i=1; i<packet.length(); i++)
    {
        pie_chart_plot_item_t item;

        item.name = packet[i];

        QRegExp rx("\\d{1,},\\d{1,},\\d{1,}");
        QRegExpValidator v(rx,this);

        QStringList values;
        if(packet[i+1].contains(rx)) //This set has a color specified
        {
            QStringList color = packet[i+1].split(",");
            item.color = QColor(color[0].toInt(),color[1].toInt(),color[2].toInt());
            item.colorIsSet = true;
            item.value = packet[i+2].toDouble();

            i += 2;
        }
        else
        {
            item.colorIsSet = false;
            item.value = packet[i+1].toDouble();
            i += 1;
        }

        items.append( item );

    }

    plot();
}

void PieChart::plot()
{
    chart->removeAllSeries();

    chart->setAnimationOptions( (QChart::AnimationOption)ui->animationComboBox->currentIndex() );
    ui->chartView->setRenderHint(QPainter::Antialiasing, ui->antialiasingCheckBox->isChecked());

    //create series
    QPieSeries * series = new QPieSeries();
//    series->setLabelsVisible();

    series->setHorizontalPosition( ui->hPosSpinBox->value() );
    series->setVerticalPosition( ui->vPosSpinBox->value() );
    series->setPieSize( ui->sizeFactorSpinBox->value() );
    series->setPieStartAngle( ui->startAngleSpinBox->value() );
    series->setPieEndAngle( ui->endAngleSpinBox->value() );
    series->setHoleSize( ui->holeSizeSpinBox->value() );

    foreach( pie_chart_plot_item_t sliceItem, items )
    {
        FlexiPieSlice * slice = new FlexiPieSlice( sliceItem.name, sliceItem.value );
        slice->setExplodeOnHover( ui->explodeSliceCheckBox->isChecked() );
        if(sliceItem.colorIsSet)
        {
            //slice->setPen(QPen(sliceItem.color, 2));
            slice->setBrush(sliceItem.color);
        }
        *series << slice;
    }

    chart->addSeries(series);

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


    //Label config
    series->setLabelsVisible( ui->showLabelsCheckBox->isChecked() );
    if(ui->showLabelsCheckBox->isChecked())
    {
        foreach( QPieSlice* slice, series->slices())
            slice->setLabelArmLengthFactor( ui->labelArmLengthSpinBox->value() );
        series->setLabelsPosition( (QPieSlice::LabelPosition)ui->labelPosComboBox->currentIndex() );
    }

    //Configure theme
    chart->setTheme( (QChart::ChartTheme) ui->themeComboBox->currentIndex() );
    QFont tFont = chart->titleFont();
    tFont.setPointSizeF( 15 );
    tFont.setBold( true );
    chart->setTitleFont( tFont );
}

void PieChart::updateDetachedLegendLayout()
{
    chart->legend()->setGeometry(QRectF(  ui->legendHPosSpinBox->value(),
                                          ui->legendVPosSpinBox->value(),
                                          ui->legendWidthSpinBox->value(),
                                          ui->legendHeightSpinBox->value()));
    chart->legend()->update();
}

void PieChart::xmlStream(QXmlStreamWriter *writer)
{
    writer->writeTextElement("id", this->id);
    writer->writeTextElement("title", ui->titleEdit->text());

    writer->writeTextElement("hpos", QString::number(ui->hPosSpinBox->value()) );
    writer->writeTextElement("vpos", QString::number(ui->hPosSpinBox->value()) );
    writer->writeTextElement("size_factor", QString::number(ui->sizeFactorSpinBox->value()) );
    writer->writeTextElement("start_angle", QString::number(ui->startAngleSpinBox->value()) );
    writer->writeTextElement("end_angle", QString::number(ui->endAngleSpinBox->value()) );
    writer->writeTextElement("hole_size", QString::number(ui->holeSizeSpinBox->value()) );
    writer->writeTextElement("explode_slice_on_hover", QString::number(ui->explodeSliceCheckBox->isChecked()) );
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
    writer->writeTextElement("label_arm_length", QString::number(ui->labelArmLengthSpinBox->value()) );
    writer->writeTextElement("labels_position", QString::number(ui->labelPosComboBox->currentIndex()) );
}

void PieChart::xmlParse(QXmlStreamReader *xml)
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

        if(xml->name() == "hpos")
        {
            double realVal = xml->readElementText().toDouble();
            ui->hPosSpinBox->setValue(realVal);
        }

        if(xml->name() == "vpos")
        {
            double realVal = xml->readElementText().toDouble();
            ui->vPosSpinBox->setValue(realVal);
        }

        if(xml->name() == "size_factor")
        {
            double realVal = xml->readElementText().toDouble();
            ui->sizeFactorSpinBox->setValue(realVal);
        }

        if(xml->name() == "start_angle")
        {
            double realVal = xml->readElementText().toDouble();
            ui->startAngleSpinBox->setValue(realVal);
        }

        if(xml->name() == "end_angle")
        {
            double realVal = xml->readElementText().toDouble();
            ui->endAngleSpinBox->setValue(realVal);
        }

        if(xml->name() == "hole_size")
        {
            double realVal = xml->readElementText().toDouble();
            ui->holeSizeSpinBox->setValue(realVal);
        }

        if(xml->name() == "explode_slice_on_hover")
        {
            int intVal = xml->readElementText().toInt();
            if(intVal > 0)
                ui->explodeSliceCheckBox->setCheckState( Qt::Checked );
            else
                ui->explodeSliceCheckBox->setCheckState( Qt::Unchecked );
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

        if(xml->name() == "label_arm_length")
        {
            double realVal = xml->readElementText().toDouble();
            ui->labelArmLengthSpinBox->setValue(realVal);
        }

        if(xml->name() == "labels_position")
        {
            int intVal = xml->readElementText().toInt();
            ui->labelPosComboBox->setCurrentIndex(intVal);
        }





        /* ...and next... */
        xml->readNext();
    }

    blockAllSignals(false);
    settingsChanged();
}

void PieChart::setId(QString str)
{
    id = str;
    this->setWindowTitle(QString("%1 \t|\t ID: %2").arg(ui->titleEdit->text()).arg(str));

    ui->idEdit->blockSignals(true);
    ui->idEdit->setText( str );
    ui->idEdit->blockSignals(false);

    Config::getInstance()->setUnsavedChanges(true);
}

void PieChart::setTitle(QString str)
{
    chart->setTitle( str );
    this->setWindowTitle(QString("%1 \t|\t ID: %2").arg(str).arg(id));
    Config::getInstance()->setUnsavedChanges(true);
}

void PieChart::blockAllSignals(bool block)
{
    ui->idEdit->blockSignals(block);
    ui->titleEdit->blockSignals(block);

    ui->hPosSpinBox->blockSignals(block);
    ui->vPosSpinBox->blockSignals(block);
    ui->sizeFactorSpinBox->blockSignals(block);
    ui->startAngleSpinBox->blockSignals(block);
    ui->endAngleSpinBox->blockSignals(block);
    ui->holeSizeSpinBox->blockSignals(block);
    ui->explodeSliceCheckBox->blockSignals(block);
    ui->animationComboBox->blockSignals(block);
    ui->themeComboBox->blockSignals(block);
    ui->antialiasingCheckBox->blockSignals(block);

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

    ui->labelArmLengthSpinBox->blockSignals(block);
    ui->labelPosComboBox->blockSignals(block);
    ui->showLabelsCheckBox->blockSignals(block);
}

PieChart::~PieChart()
{
    delete ui;
}

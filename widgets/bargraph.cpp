#include "bargraph.h"
#include "ui_bargraph.h"

QT_CHARTS_USE_NAMESPACE

BarGraph::BarGraph(QWidget *parent) :
    AbstractWidget(parent),
    ui(new Ui::BarGraph)
{
    ui->setupUi(this);


    QBarSet *set0 = new QBarSet("Jane");
    QBarSet *set1 = new QBarSet("John");
    QBarSet *set2 = new QBarSet("Axel");
    QBarSet *set3 = new QBarSet("Mary");
    QBarSet *set4 = new QBarSet("Samantha");

    *set0 << 1 << 2 << 3 << 4 << 5 << 6;
    *set1 << 5 << 0 << 0 << 4 << 0 << 7;
    *set2 << 3 << 5 << 8 << 13 << 8 << 5;
    *set3 << 5 << 6 << 7 << 3 << 4 << 5;
    *set4 << 9 << 7 << 5 << 3 << 1 << 2;

    QBarSeries *series = new QBarSeries();
    series->append(set0);
    series->append(set1);
    series->append(set2);
    series->append(set3);
    series->append(set4);


    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Simple barchart example");
    chart->setAnimationOptions(QChart::SeriesAnimations);

    QStringList categories;
    categories << "Jan" << "Feb" << "Mar" << "Apr" << "May" << "Jun";
    QBarCategoryAxis *axis = new QBarCategoryAxis();
    axis->append(categories);
    chart->createDefaultAxes();
    chart->setAxisX(axis, series);

    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);

//    QChartView *chartView = new QChartView(chart);
    ui->chartView->setChart( chart );
    ui->chartView->setRenderHint(QPainter::Antialiasing);

//    this->layout()->addWidget( chartView );

    this->setWindowTitle("Bar Graph 1");
}

bool BarGraph::validPacket(QString packet)
{
    QRegExp rx_timeplot("[a-zA-Z0-9]+(\\|[a-zA-Z0-9 ]+\\|\\d{1,3},\\d{1,3},\\d{1,3}\\|\\-*\\d+(\\.{0,1}\\d+)*)+");
    QRegExp rx_xy_plot("[a-zA-Z0-9]+\\|[a-zA-Z0-9 ]+\\|\\d{1,3},\\d{1,3},\\d{1,3}\\|(\\-*\\d+\\s\\-*\\d+\\s*)+");

    return rx_timeplot.exactMatch(packet) || rx_xy_plot.exactMatch(packet);
}

void BarGraph::serialPacket(QStringList packet)
{

}

void BarGraph::xmlStream(QXmlStreamWriter *writer)
{

}

void BarGraph::xmlParse(QXmlStreamReader *xml)
{
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


        /* ...and next... */
        xml->readNext();
    }
}

void BarGraph::reset()
{

}

void BarGraph::setId(QString str)
{
   id = str;
   this->setWindowTitle(QString("%1 \t|\t ID: %2").arg("ui->titleEdit->text()").arg(str));


   Config::getInstance()->setUnsavedChanges(true);

}

BarGraph::~BarGraph()
{
    delete ui;
}

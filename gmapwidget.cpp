#include "gmapwidget.h"

#include <QXmlStreamReader>
#include <QNetworkRequest>
#include <QtWebKitWidgets/QWebFrame>
//http://qt-project.org/forums/viewthread/12342 <-- Source was downloaded from here, and JUST
// some functionality was added as necessary, but the core is developed by steven.
GMapWidget::GMapWidget(QWidget *parent)
    : QWebView(parent)
{
    m_accessManager = new QNetworkAccessManager(this);
    m_initMap = false;

    connect(m_accessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(slotNetworkReply(QNetworkReply*)));

    connect(this, SIGNAL(loadFinished(bool)), this, SLOT(slotLoadFinished(bool)));

    load(QUrl("qrc:/index.html"));
}

void GMapWidget::loadMap()
{
   load(QUrl("qrc:/index.html"));


}

// wait till the load finished signal before initializing the map trough javascript functions!
// somethimes the geocoding is faster than the webview can load the google map runtime
void GMapWidget::slotLoadFinished(bool ok)
{
    if(!m_initMap && ok) {
        m_initMap = true;

        // load a default location on map init
        setGMapCenter("Riga, Latvia");
    }
}

// geocode location strings to Latitude / Longitude positions
void GMapWidget::setGMapCenter(const QString &location)
{
    QUrl url("http://maps.googleapis.com/maps/api/geocode/xml");

    QUrlQuery q;
    q.addQueryItem("address", location);
    q.addQueryItem("sensor", "false");

    url.setQuery( q );

    QNetworkRequest request(url);

    m_accessManager->get(request);


}

// overload with QPointF
void GMapWidget::setGMapCenter(const QPointF &pos)
{
    setGMapCenter(pos.x(), pos.y());
}

// overload with qreal
void GMapWidget::setGMapCenter(qreal lat, qreal lng)
{
    runScript(QString("setGMapCenter(%1,%2)").arg(lat).arg(lng));
}

// set a zoom level on the map
void GMapWidget::setGMapZoom(int zoomLevel)
{
    runScript(QString("setGMapZoom(%1)").arg(zoomLevel));
}

void GMapWidget::addMarker(float lat, float lng, QString name)
{
    runScript( QString("newMarker(%1,%2,'%3')").arg(lat).arg(lng).arg(name) );
}

// read out a geocode xml reply from google
void GMapWidget::slotNetworkReply(QNetworkReply *reply)
{
    QXmlStreamReader reader(reply->readAll());

    while(!reader.atEnd()) {

        reader.readNext();

        if(reader.name() == "geometry") {

            reader.readNextStartElement();

            if(reader.name() == "location") {

                reader.readNextStartElement();

                if(reader.name() == "lat") {

                    QPointF pos;

                    pos.setX(reader.readElementText().toFloat());

                    reader.readNextStartElement();

                    if(reader.name() == "lng") {

                        pos.setY(reader.readElementText().toFloat());

                        setGMapCenter(pos);

                        return;
                    }
                }
            }
        }
    }
}

// run a javascript function
void GMapWidget::runScript(const QString &script)
{
    page()->mainFrame()->evaluateJavaScript(script);
}

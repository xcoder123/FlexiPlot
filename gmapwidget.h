#ifndef GMAPWIDGET_H
#define GMAPWIDGET_H

#include <QtWebKitWidgets/QWebView>

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QUrlQuery>
#include <QWidget>

class GMapWidget : public QWebView
{
    Q_OBJECT
public:
    explicit GMapWidget(QWidget *parent = 0);

public slots:
    void addMarker(float lat, float lng, QString name);

    void setGMapCenter(const QString &location);
    void setGMapCenter(const QPointF &pos);
    void setGMapCenter(qreal lat, qreal lng);

    void setGMapZoom(int zoomLevel);

    void loadMap();

private slots:
    void slotNetworkReply(QNetworkReply *reply);
    void slotLoadFinished(bool ok);

private:
    void runScript(const QString &script);

private:
    QNetworkAccessManager *m_accessManager;
    bool m_initMap;
};

#endif // GMAPWIDGET_H

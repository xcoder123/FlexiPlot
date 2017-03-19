#ifndef FLEXIPIESLICE_H
#define FLEXIPIESLICE_H

#include <QtCharts/QPieSlice>
#include <QBrush>

QT_CHARTS_USE_NAMESPACE

class FlexiPieSlice : public QPieSlice
{
    Q_OBJECT

public:
    FlexiPieSlice(QString label, qreal value);
    void setExplodeOnHover(bool b) { explodeOnHover = b; }

public:
    QBrush originalBrush();

public Q_SLOTS:
    void showHighlight(bool show);

private:
    QBrush m_originalBrush;
    bool explodeOnHover;

};

#endif // FLEXIPIESLICE_H

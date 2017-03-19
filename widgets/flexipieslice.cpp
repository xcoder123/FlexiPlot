#include "flexipieslice.h"


QT_CHARTS_USE_NAMESPACE

FlexiPieSlice::FlexiPieSlice(QString label, qreal value)
    : QPieSlice(label, value)
{
    connect(this, SIGNAL(hovered(bool)), this, SLOT(showHighlight(bool)));
}

QBrush FlexiPieSlice::originalBrush()
{
    return m_originalBrush;
}

void FlexiPieSlice::showHighlight(bool show)
{
    if (show) {
        QBrush brush = this->brush();
        m_originalBrush = brush;
        brush.setColor(brush.color().lighter());
        setBrush(brush);
        if(explodeOnHover)
            setExploded(true);
    } else {
        setBrush(m_originalBrush);
        if(explodeOnHover)
            setExploded(false);
    }
}

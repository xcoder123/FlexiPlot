#ifndef HELPER_H
#define HELPER_H

//#include "linegraph.h"
#include "linechart.h"
#include "bargraph.h"
#include "piechart.h"
#include "scatterchart.h"

class Helper
{
public:
    Helper();

    static bool validPacket(QString packet, int type=0);
};

#endif // HELPER_H

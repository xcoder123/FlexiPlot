#include "helper.h"

Helper::Helper()
{

}

bool Helper::validPacket(QString packet, int type)
{
    if(packet.isEmpty())
        return false;

    if(type == 0) //check for any object
    {
        return LineGraph::validPacket(packet) || BarGraph::validPacket(packet);
    }
    else
    {
        bool packetValid = false;
        switch(type) {
            case LineGraph::Type: packetValid = LineGraph::validPacket(packet); break;
            case BarGraph::Type: packetValid = BarGraph::validPacket(packet); break;
            default: packetValid = false;
        }

        return packetValid;
    }
}

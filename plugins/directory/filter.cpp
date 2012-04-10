#include "filter.h"

// filter.cpp
QString Filter::getString()
{
    return "Qt Directory Filter Plugins";
}

// export the plugin
// the TARGET should be "filter" also
Q_EXPORT_PLUGIN2(_directoryFilter, Filter);

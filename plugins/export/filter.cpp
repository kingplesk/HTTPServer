#include "filter.h"

// filter.cpp
QString Filter::getString()
{
    return "Qt Export Filter Plugins";
}

// export the plugin
// the TARGET should be "filter" also
Q_EXPORT_PLUGIN2(pnp_exportFilter, Filter);

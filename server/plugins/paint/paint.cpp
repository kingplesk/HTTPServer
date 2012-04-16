#include "paint.h"

// paint.cpp
QString Paint::getString()
{
    return "Qt Paint Plugins";
}

// export the plugin
// the TARGET should be "paint" also
Q_EXPORT_PLUGIN2(_paint, Paint);

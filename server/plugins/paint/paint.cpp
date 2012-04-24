#include "paint.h"

// paint.cpp
QString Paint::getString()
{
    qDebug() << m_items;
    qDebug() << m_params;

    qDebug() << items();
    qDebug() << params();

    return "Qt Paint Plugins";
}

QStringList Paint::items() const
{
    return m_items;
}

QStringList Paint::params() const
{
    return m_params;
}

void Paint::setItems(const QStringList &items)
{
    m_items = items;
}

void Paint::setParams(const QStringList &params)
{
    m_params = params;
}


// export the plugin
// the TARGET should be "paint" also
Q_EXPORT_PLUGIN2(_paint, Paint);

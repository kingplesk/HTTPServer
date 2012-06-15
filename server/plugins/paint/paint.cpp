#include "paint.h"

// paint.cpp
QString Paint::getString()
{
    qDebug() << m_items;
    qDebug() << m_params;
    qDebug() << m_channels;

    qDebug() << items();
    qDebug() << params();
    qDebug() << channels();

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

QStringList Paint::channels() const
{
    return m_channels;
}

void Paint::setItems(const QStringList &items)
{
    m_items = items;
}

void Paint::setParams(const QStringList &params)
{
    m_params = params;
}

void Paint::addChannel(const QStringList &channel)
{
    m_channels << channel;
}


// export the plugin
// the TARGET should be "paint" also
Q_EXPORT_PLUGIN2(_paint, Paint);

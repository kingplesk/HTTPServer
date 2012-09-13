#include "paint.h"

Paint::Paint(QObject * parent) :
    QObject(parent)
{
}

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

void Paint::setChannel(QString channel)
{
    this->channel = channel;
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

QVariantMap Paint::maps() const
{
    return m_maps;
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

void Paint::setMap(const QVariantMap &variant)
{
    QVariantMap::const_iterator i = variant.constBegin();

    qDebug() << "setMap data:" << variant;

    m_maps.insert(channel, i.value());
}

QObject * Paint::painted()
{
    qDebug() << "painted";
    //return QString("painted ReturnValue");

    QObject * object = dynamic_cast<QObject *>(this);

    return object;
}

QObject * Paint::newItem()
{
    qDebug() << "newItem";
    //return QString("newItem ReturnValue");

    QObject * object = dynamic_cast<QObject *>(this);

    return object;
}

QObject * Paint::newTest()
{
    qDebug() << "QObject * Paint::newTest()";

    QObject * object = dynamic_cast<QObject *>(new Painted());

    return object;
}

QObject * Paint::newMap()
{
    qDebug() << "QObject * Paint::setMap()";

    QObject * object = dynamic_cast<QObject *>(this);

    return object;
}

QObject * Paint::init()
{
    qDebug() << "QObject * Paint::init()";

    QObject * object = dynamic_cast<QObject *>(this);

    return object;
}


// export the plugin
// the TARGET should be "paint" also
Q_EXPORT_PLUGIN2(_paint, Paint);

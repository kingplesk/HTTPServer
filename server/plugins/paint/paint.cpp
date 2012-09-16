#include "paint.h"

Paint::Paint(QObject * parent) :
    QObject(parent)
{
}

// paint.cpp
QString Paint::getString()
{
    return "Qt Paint Plugins";
}

void Paint::setChannel(QString channel)
{
    //this->channel = channel;
}

QVariantMap * Paint::newMap(const QVariantMap & data)
{
    qDebug() << "QObject * Paint::newMap()";

    QObject * object = dynamic_cast<QObject *>(this);

    return new QVariantMap();
}

QVariantMap * Paint::joinMap(const QVariantMap & data)
{
    qDebug() << "QObject * Paint::joinMap()";

    QObject * object = dynamic_cast<QObject *>(this);

    return new QVariantMap();
}

QVariantMap * Paint::painted(const QVariantMap & data)
{
    qDebug() << "painted";
    //return QString("painted ReturnValue");

    QObject * object = dynamic_cast<QObject *>(this);

    return new QVariantMap();
}

QVariantMap * Paint::init()
{
    qDebug() << "QObject * Paint::init()";

    QObject * object = dynamic_cast<QObject *>(this);

    return new QVariantMap();
}

/*
void Paint::setMap(const QVariantMap &variant)
{
    QVariantMap::const_iterator i = variant.constBegin();

    qDebug() << "setMap data:" << variant;

    m_maps.insert(channel, i.value());
}
*/
/*
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
*/

// export the plugin
// the TARGET should be "paint" also
Q_EXPORT_PLUGIN2(_paint, Paint);

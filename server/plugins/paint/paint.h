#ifndef PAINT_H
#define PAINT_H

#include <QDate>
#include <QDebug>
#include <QMap>
#include <QStringList>
#include <QObject>
#include <QVariant>

#include "paint.h"
#include "painted.h"
#include "server/myinterface.h"

class Paint : public QObject, public MyInterface
{
    Q_OBJECT

    Q_PROPERTY(QStringList items READ items WRITE setItems)
    Q_PROPERTY(QStringList params READ params WRITE setParams)
    Q_PROPERTY(QStringList channels READ channels WRITE addChannel)
    Q_PROPERTY(QVariantMap maps READ maps WRITE setMap)

    // register to Qt's meta-object system
    Q_INTERFACES(MyInterface)

public:
    Paint(QObject * parent = 0);
    virtual QString getString();
    virtual void setChannel(QString channel);

    QStringList items() const;
    void setItems(const QStringList& items);

    QStringList params() const;
    void setParams(const QStringList& params);

    QStringList channels() const;
    void addChannel(const QStringList& channel);

    QVariantMap maps() const;
    void setMap(const QVariantMap &variant);

public slots:
    QObject * painted();
    QObject * newItem();
    QObject * newTest();
    QObject * newMap();
    QObject * init();

private:
    QStringList m_items;
    QStringList m_params;
    QStringList m_channels;
    QString channel;
    QMap<QString, QVariant> m_maps;
    //Painted * painted_;
};

#endif // PAINT_H

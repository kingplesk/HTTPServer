#ifndef PAINT_H
#define PAINT_H

#include <QDate>
#include <QDebug>
#include <QStringList>
#include <QObject>

#include "paint.h"
#include "painted.h"
#include "server/myinterface.h"

class Paint : public QObject, public MyInterface
{
    Q_OBJECT

    Q_PROPERTY(QStringList items READ items WRITE setItems)
    Q_PROPERTY(QStringList params READ params WRITE setParams)
    Q_PROPERTY(QStringList channels READ channels WRITE addChannel)

    // register to Qt's meta-object system
    Q_INTERFACES(MyInterface)

public:
    Paint(QObject * parent = 0);
    virtual QString getString();

    QStringList items() const;
    void setItems(const QStringList& items);

    QStringList params() const;
    void setParams(const QStringList& params);

    QStringList channels() const;
    void addChannel(const QStringList& channel);

public slots:
    QObject * painted();
    QObject * newItem();
    QObject * newTest();

private:
    QStringList m_items;
    QStringList m_params;
    QStringList m_channels;
    //Painted * painted_;
};

#endif // PAINT_H

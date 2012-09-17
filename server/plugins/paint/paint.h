#ifndef PAINT_H
#define PAINT_H

#include <QDebug>
#include <QObject>
#include <QVariantMap>

#include "paint.h"
#include "server/myinterface.h"

class Paint : public QObject, public MyInterface
{
    Q_OBJECT

    // register to Qt's meta-object system
    Q_INTERFACES(MyInterface)

public:
    Paint(QObject * parent = 0);
    virtual QString getString();
    virtual void setChannel(QString channel);

public slots:
    QVariantMap newMap(const QVariantMap &);
    QVariantMap joinMap(const QVariantMap &);
    QVariantMap painted(const QVariantMap &);
    QVariantMap init();

private:
    QMap<int, QVariant> maps;
};

#endif // PAINT_H

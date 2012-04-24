#ifndef PAINT_H
#define PAINT_H

#include <QDate>
#include <QDebug>
#include <QStringList>
#include <QObject>

#include "paint.h"
#include "server/myinterface.h"

class Paint : public QObject, public MyInterface
{
    Q_OBJECT

    Q_PROPERTY(QStringList items READ items WRITE setItems)
    Q_PROPERTY(QStringList params READ params WRITE setParams)

    // register to Qt's meta-object system
    Q_INTERFACES(MyInterface)

public:
    virtual QString getString();

    QStringList items() const;
    void setItems(const QStringList& items);

    QStringList params() const;
    void setParams(const QStringList& params);

private:
    QStringList m_items;
    QStringList m_params;
};

#endif // PAINT_H

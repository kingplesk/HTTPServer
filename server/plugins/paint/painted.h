#ifndef PAINTED_H
#define PAINTED_H

#include <painted.h>

#include <QObject>
#include <QStringList>
#include <QDebug>

class Painted : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QStringList items READ items WRITE setItems)
    Q_PROPERTY(QStringList params READ params WRITE setParams)

public:
    explicit Painted(QObject *parent = 0);

    QStringList items() const;
    void setItems(const QStringList& items);

    QStringList params() const;
    void setParams(const QStringList& params);

private:
    QStringList m_items;
    QStringList m_params;
};

#endif // PAINTED_H

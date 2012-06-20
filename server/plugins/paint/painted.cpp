#include "painted.h"

Painted::Painted(QObject *parent) :
    QObject(parent)
{
    qDebug() << "Painted.cpp";
}

QStringList Painted::items() const
{
    return m_items;
}

QStringList Painted::params() const
{
    return m_params;
}

void Painted::setItems(const QStringList &items)
{
    m_items = items;
}

void Painted::setParams(const QStringList &params)
{
    m_params = params;
}

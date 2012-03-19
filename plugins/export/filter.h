#ifndef FILTER_H
#define FILTER_H

#include <QObject>

#include "filter.h"
#include "server/myinterface.h"

class Filter : public QObject, public MyInterface
{
    Q_OBJECT

    // register to Qt's meta-object system
    Q_INTERFACES(MyInterface)

public:
    virtual QString getString();
};

#endif // FILTER_H

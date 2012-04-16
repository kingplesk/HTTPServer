#ifndef PAINT_H
#define PAINT_H

#include <QObject>

#include "paint.h"
#include "server/myinterface.h"

class Paint : public QObject, public MyInterface
{
    Q_OBJECT

    // register to Qt's meta-object system
    Q_INTERFACES(MyInterface)

public:
    virtual QString getString();
};

#endif // PAINT_H

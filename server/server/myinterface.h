#ifndef MYINTERFACE_H
#define MYINTERFACE_H

#include "myinterface.h"

#include <QString>
#include <QtPlugin>

class MyInterface
{
public:
    virtual ~MyInterface() {}

    virtual QString getString() = 0;
    virtual void setChannel(QString channel) = 0;
};

// associate MyInterface with the unique identifier
// should not be done within any namespace
Q_DECLARE_INTERFACE(MyInterface, "com.eri.MyInterface/1.0")

#endif // MYINTERFACE_H

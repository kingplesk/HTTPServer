#ifndef TEST1_H
#define TEST1_H

#include <QObject>

class Test1 : public QObject
{
    Q_OBJECT

public:
    explicit Test1(QObject *parent = 0);
    QString test(QString p);
};

#endif // TEST1_H

#ifndef TEST_H
#define TEST_H

#include <QObject>

class Test : public QObject
{
    Q_OBJECT
public:
    explicit Test(QObject *parent = 0);

signals:

public slots:
    QString test();
};

#endif // TEST_H

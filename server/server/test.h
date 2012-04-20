#ifndef TEST_H
#define TEST_H

#include <QObject>
#include <QVector>

class Test : public QObject
{
Q_OBJECT

Q_PROPERTY(QString string1)
Q_PROPERTY(qint32 int1)

public:
    explicit Test(QObject *parent = 0);

signals:

public slots:
    QString test();

public:
    QString string1;
    QVector<QString> string2;
    qint32 int1;
    QObject object1;
    bool bool1;
};

#endif // TEST_H

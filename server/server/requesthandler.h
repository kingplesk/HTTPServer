#ifndef REQUESTHANDLER_H
#define REQUESTHANDLER_H

#include <QObject>

class RequestHandler : public QObject
{
    Q_OBJECT
public:
    explicit RequestHandler(QObject *parent = 0);

signals:

public slots:

};

#endif // REQUESTHANDLER_H

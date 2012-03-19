#ifndef CLIENTHANDLER_H
#define CLIENTHANDLER_H

#include <QObject>
#include <QByteArray>

class ClientHandler : public QObject
{
    Q_OBJECT
public:
    explicit ClientHandler(QObject *parent = 0);
    QByteArray getHandler(QByteArray snippet);

signals:

public slots:

};

#endif // CLIENTHANDLER_H

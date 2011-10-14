#ifndef HTTPREQUEST_H
#define HTTPREQUEST_H

#include <QByteArray>
#include <QObject>
#include "clienthandler.h"

class HTTPRequest : public QObject
{
    Q_OBJECT

    public:
        explicit HTTPRequest(QObject * parent = 0);
        void requestReady(QByteArray);

    signals:
        void newRequest(QByteArray request);

    private:
        ClientHandler clientHandler_;

};

#endif // HTTPREQUEST_H

#ifndef HTTPREQUEST_H
#define HTTPREQUEST_H

#include <QObject>
#include <QByteArray>
#include <QHttpRequestHeader>

#include "comet.h"
#include "clienthandler.h"

class HttpRequest : public QObject
{
    Q_OBJECT

    public:
        explicit HttpRequest(QObject * parent = 0);
        void requestReady(QHttpRequestHeader header, QByteArray body);

    signals:
        void newRequest(QHttpRequestHeader header, QByteArray body);

    private:
        ClientHandler clientHandler_;
        Comet * comet_;

};

#endif // HTTPREQUEST_H

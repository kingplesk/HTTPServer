#ifndef CLIENTSOCKET_H
#define CLIENTSOCKET_H

#include "httprequest.h"
#include "httpresponse.h"
#include "httpparser.h"

#include <QTcpSocket>

class ClientSocket : public QTcpSocket
{
    Q_OBJECT

    public:
        explicit ClientSocket(QObject * parent, HttpRequest * request, HttpResponse * response, HttpParser * parser);

    public slots:
        void readClient(QHttpRequestHeader header, QByteArray body);
        void send(QByteArray response);

    private:
        HttpRequest * request_;
        HttpResponse * response_;
        HttpParser * parser_;

};

#endif // CLIENTSOCKET_H

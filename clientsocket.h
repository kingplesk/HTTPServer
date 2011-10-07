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
        explicit ClientSocket(QObject * parent, HTTPRequest * request, HTTPResponse * response, HTTPParser * parser);

    public slots:
        void readClient(QByteArray snippet);
        void send(QByteArray response);

    private:
        HTTPRequest * request_;
        HTTPResponse * response_;
        HTTPParser * parser_;

};

#endif // CLIENTSOCKET_H

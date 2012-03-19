#ifndef SERVER_H
#define SERVER_H

#include "httprequest.h"
#include "httpresponse.h"
#include "httpparser.h"
#include "clientsocket.h"

#include <QTcpServer>
#include <QTcpSocket>
#include <QByteArray>
#include <QObject>


class Server : public QTcpServer
{
    Q_OBJECT;

    public:
        Server(QObject * parent = 0);
        void start(qint16 port = 8888);

    protected:
        void incomingConnection(int descriptor);

    private:
        HTTPRequest request_;
        HTTPResponse response_;
        HTTPParser parser_;


};

#endif // SERVER_H

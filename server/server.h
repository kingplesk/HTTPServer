#ifndef SERVER_H
#define SERVER_H

//#include "httprequest.h"
//#include "httpresponse.h"
//#include "httpparser.h"

#include "http.h"
#include "clienthandler.h"

#include <QMap>
#include <QObject>
#include <QTcpServer>
//#include <QTcpSocket>
//#include <QByteArray>

class Server : public QObject
{
    Q_OBJECT;

    public:
        Server(QObject * parent = 0);
        void start(qint16 port = 8888);
        void sendReply(QByteArray response);

    protected:
        //void incomingConnection(int descriptor);

    signals:
        void newRequest();

    protected slots:
        //void newRequest(QHttpRequestHeader header, QByteArray body);
        void handle();
        void test();

    private:
        //Http http_;
        //HttpRequest request_;
        //HttpResponse response_;
        //HttpParser parser_;
        //QTcpSocket * socket_;
        int i_;
        QMap<QString, ClientHandler *> clients_;
        QTcpServer * server_;
};

#endif // SERVER_H

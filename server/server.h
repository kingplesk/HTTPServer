#ifndef SERVER_H
#define SERVER_H

#include "http.h"
#include "clienthandler.h"
#include "requesthandler.h"

#include <QMap>
#include <QObject>
#include <QTcpServer>

class Server : public QObject
{
    Q_OBJECT

    public:
        Server(QObject * parent = 0);
        ~Server();

        void start(qint16 port = 8888);
        void sendReply(QByteArray response);

    signals:
        void newRequest();

    protected slots:
        void handle();
        void newConnection();

    private:
        int i_;
        QMap<QString, ClientHandler *> clients_;
        QTcpServer * server_;
        RequestHandler * requestHandler_;

};

#endif // SERVER_H

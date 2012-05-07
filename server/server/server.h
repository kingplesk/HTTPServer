#ifndef SERVER_H
#define SERVER_H

#include <QDir>
#include <QMap>
#include <QTimer>
#include <QObject>
#include <QTcpServer>
#include <QPluginLoader>

#include "http.h"
#include "clienthandler.h"
#include "requesthandler.h"

class Server : public QObject
{
    Q_OBJECT

    public:
        Server(QObject * parent = 0);
        ~Server();

        void start(qint16 port = 8888);
        void sendReply(QByteArray response);
        void loadPlugins();

    signals:
        void newRequest();

    protected slots:
        void handle();
        void update();
        void newConnection();
        void broadcast(QString json);
        void deleteClientHandler(QString uuid);

    private:
        int i_;
        QMap<QString, ClientHandler *> clients_;
        QTcpServer server_;
        RequestHandler requestHandler_;
        QTimer timer_;

        QDir pluginsDir_;
        QMap<QString, MyInterface *> plugins_;
        QMap<QString, QPluginLoader *> p_;
};

#endif // SERVER_H

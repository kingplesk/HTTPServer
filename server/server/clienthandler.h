#ifndef CLIENTHANDLER_H
#define CLIENTHANDLER_H

#include <QMap>
#include <QTimer>
#include <QObject>
#include <QStringList>
#include <QDateTime>
#include <QByteArray>
#include <QPluginLoader>

#include "http.h"
#include "myinterface.h"



struct comet {
    Http * http;
    QTimer * timer;
    int lastMessageId;
    QDateTime lastUpdated;
    QString tid;
};

class ClientHandler : public QObject
{
    Q_OBJECT

    public:
        template<typename Func>
        void cometIterator(Func func)
        {
            for(QMap<QString, comet>::iterator it = comets_.begin(); it != comets_.end(); ++it) {
                comet& nextComet = it.value();
                func(nextComet);
            }
        }

        ClientHandler(QObject * parent = 0);
        void sendComet(QString json);
        void sendComet(comet& nextComet);
        void newComet(Http * http, QMap<QString, QPluginLoader *>& p);
        void newRequest(Http * http, QMap<QString, QPluginLoader *>& p);
        void checkState();
        bool cometsEmpty();
        comet& getComet(QTimer * timer);
        comet& getComet(Http * http);
        comet& getComet(QString tid);
        MyInterface * createPluginInstance(QString pluginName, QMap<QString, QPluginLoader *>& p);

        QString uuid;
        QDateTime lastUpdated;

    signals:
        void broadcast(QString json);
        void deleteClientHandler(QString uuid);

    public slots:
        void closeComet();

    private:
        QTimer * timer_;
        QMap<QString, comet> comets_;
        QStringList messageQueue_;
};

#endif // CLIENTHANDLER_H

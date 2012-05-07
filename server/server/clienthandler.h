#ifndef CLIENTHANDLER_H
#define CLIENTHANDLER_H

#include <QMap>
#include <QTimer>
#include <QObject>
#include <QVector>
#include <QDateTime>
#include <QMutableVectorIterator>
#include <QByteArray>
#include <QPluginLoader>

#include "http.h"
#include "myinterface.h"

class ClientHandler : public QObject
{
    Q_OBJECT

    public:
        ClientHandler(QObject * parent = 0);
        void sendComet(QString json);
        void newComet(Http * http, QMap<QString, QPluginLoader *>& p);
        void newRequest(Http * http, QMap<QString, QPluginLoader *>& p);
        void checkState();
        MyInterface * createPluginInstance(QString pluginName, QMap<QString, QPluginLoader *>& p);
        MyInterface * createPluginInstance(QString pluginName, QMap<QString, QPluginLoader *>& p, QVariantMap map);

        qint16 i_;
        QString uuid;
        QDateTime lastUpdated;
    signals:
        void broadcast(QString json);
        void deleteClientHandler(QString uuid);

    public slots:
        void closeComet();

    private:
        QTimer * timer_;
        QMap<Http *,  QTimer *> comets_;
        QVector<Http *> requests_;
        void * newValue_;
};

#endif // CLIENTHANDLER_H

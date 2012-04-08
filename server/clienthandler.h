#ifndef CLIENTHANDLER_H
#define CLIENTHANDLER_H

#include <QMap>
#include <QObject>
#include <QTimer>
#include <QByteArray>
#include <QPluginLoader>

#include "http.h"
#include "myinterface.h"

class ClientHandler : public QObject
{
    Q_OBJECT

    public:
        ClientHandler(QObject * parent = 0);
        void newComet(Http * http, QMap<QString, QPluginLoader *>& p);
        void newRequest(Http * http, QMap<QString, QPluginLoader *>& p);
        MyInterface * createPluginInstance(QString pluginName, QMap<QString, QPluginLoader *>& p);

        qint16 i_;

    signals:

    public slots:

    private:
        QTimer * timer_;

};

#endif // CLIENTHANDLER_H


#include "clienthandler.h"

ClientHandler::ClientHandler(QObject * parent) :
    QObject(parent),
    comets_(0),
    requests_(0)
{
    i_ = 0;
}

void ClientHandler::sendComet(QString json)
{
    QVectorIterator<Http *> i(comets_);
    while (i.hasNext()) {
        i.next()->sendReply(QByteArray().append(json));
    }
}

void ClientHandler::newComet(Http * http, QMap<QString, QPluginLoader *>& p)
{
    comets_.append(http);

    timer_ = new QTimer(this);
    //timer_->setSingleShot(true);
    //timer_->setInterval(30 * 1000);
    timer_->singleShot(30 * 1000, http, SLOT(closeComet()));
}

void ClientHandler::newRequest(Http * http, QMap<QString, QPluginLoader *>& p)
{
    requests_.append(http);

    MyInterface *mi = createPluginInstance("libpnp_directoryFilter.so", p);

    qDebug() << "newRequest";

    http->sendReply(QByteArray("Reply Ajax: ").append(mi->getString()));
}

MyInterface * ClientHandler::createPluginInstance(QString pluginName, QMap<QString, QPluginLoader *>& p)
{
    QObject *plugin = p[pluginName]->instance();
    MyInterface *mi = qobject_cast<MyInterface *>(plugin);

    return mi;
}


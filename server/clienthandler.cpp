
#include "clienthandler.h"

ClientHandler::ClientHandler(QObject * parent) :
    QObject(parent)
{
    i_ = 0;
}

void ClientHandler::newComet(Http * http, QMap<QString, QPluginLoader *>& p)
{
    timer_ = new QTimer(this);
    //timer_->setSingleShot(true);
    //timer_->setInterval(30 * 1000);
    timer_->singleShot(30 * 1000, http, SLOT(closeComet()));
}

void ClientHandler::newRequest(Http * http, QMap<QString, QPluginLoader *>& p)
{
    MyInterface *mi = createPluginInstance("libpnp_directoryFilter.so", p);

    http->sendReply(QByteArray("Reply Ajax: ").append(mi->getString()));
}

MyInterface * ClientHandler::createPluginInstance(QString pluginName, QMap<QString, QPluginLoader *>& p)
{
    QObject *plugin = p[pluginName]->instance();
    MyInterface *mi = qobject_cast<MyInterface *>(plugin);

    return mi;
}


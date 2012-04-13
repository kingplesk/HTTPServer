
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
    QMutableVectorIterator<Http *> i(comets_);
    while (i.hasNext()) {
        Http * next = i.next();
        QAbstractSocket::SocketState state = next->state();
        qDebug() << "TEST -----" << state;
        if (state == QAbstractSocket::ConnectedState) {
            next->sendReply(QByteArray().append(json));
        }
        else {
            i.remove();
        }
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
    //requests_.append(http);
    QString reply("");
    if (p.contains("directoryFilter")) {
        MyInterface *mi = createPluginInstance("directoryFilter", p);

        qDebug() << "newRequest";

        reply.append("[0, {\"data\":\"" + mi->getString() + "\"}]");
    }
    else {
        reply.append("[1, {\"data\":\"nono-plugin\"}]");
    }

    http->sendReply(QByteArray().append(reply));
}

MyInterface * ClientHandler::createPluginInstance(QString pluginName, QMap<QString, QPluginLoader *>& p)
{
    QObject *plugin = p[pluginName]->instance();
    MyInterface *mi = qobject_cast<MyInterface *>(plugin);

    return mi;
}


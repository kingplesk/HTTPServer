
#include <QUrl>
#include <QScriptValue>
#include <QScriptEngine>
#include <QScriptValueIterator>

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

    QByteArray byteArray = QByteArray().append(http->request_->getBody());
    QString json = QUrl::fromEncoded(byteArray).toString();
    qDebug() << json;

    QScriptEngine engine;
    QScriptValue sc = engine.evaluate("(" + QString(json) + ")");

    qDebug() << sc.toString();

    QString handler;

    /*
    bool	isArray () const
    bool	isBool () const
    bool	isDate () const
    bool	isError () const
    bool	isFunction () const
    bool	isNull () const
    bool	isNumber () const
    bool	isObject () const
    bool	isUndefined () const
    bool	isString () const

    bool	toBool () const
    QDateTime	toDateTime () const
    qint32	toInt32 () const
    qsreal	toInteger () const
    qsreal	toNumber () const
    QRegExp	toRegExp () const
    QString	toString () const
    quint16	toUInt16 () const
    quint32	toUInt32 () const
    */

    QScriptValueIterator it(sc);
    while (it.hasNext()) {
        it.next();

        if (it.name() == "handler") {
            handler = it.value().toString();
        }

        qDebug() << it.name() << ": " << it.value().toString();
    }

    QString reply("");
    if (p.contains(handler)) {
        MyInterface *mi = createPluginInstance(handler, p);

        qDebug() << "newRequest";

        reply.append("[0, {\"handler\":\"" + mi->getString() + "\"}]");
    }
    else {
        reply.append("[1, {\"handler\":\"nono-plugin\"}]");
    }

    http->sendReply(QByteArray().append(reply));
}

MyInterface * ClientHandler::createPluginInstance(QString pluginName, QMap<QString, QPluginLoader *>& p)
{
    QObject *plugin = p[pluginName]->instance();
    MyInterface *mi = qobject_cast<MyInterface *>(plugin);

    return mi;
}


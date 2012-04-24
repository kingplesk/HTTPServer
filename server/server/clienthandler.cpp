
#include <QUrl>
#include <QStringList>
#include <QScriptValue>
#include <QScriptEngine>
#include <QScriptValueIterator>
#include <QRegExp>
#include <QDateTime>
#include <QVariant>
#include <QVariantMap>

#include <QJson/Parser>
#include <QJson/Serializer>
#include <QJson/QObjectHelper>

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
    QScriptValueIterator its(sc);
    while (its.hasNext()) {
        its.next();

        if (its.name() == "handler") {
            handler = its.value().toString();
        }

        qDebug() << its.name() << ": " << its.value().toString();
    }






    QByteArray qjson = QByteArray().append(json);
    QJson::Parser parser;
    bool ok;
    // json is a QString containing the data to convert
    QVariantMap variantMap = parser.parse (qjson, &ok).toMap();
    if (!ok) {
        qFatal("An error occurred during parsing");
        exit (1);
    }

    qDebug() << variantMap;

    QVariantMap variantData = variantMap["data"].toMap();

    qDebug() << variantData;




    QString reply("");
    if (p.contains(handler)) {
        MyInterface *mi = createPluginInstance(handler, p);

        qDebug() << "newRequest";

        if (handler.contains("paint")) {
            MyInterface *mi = createPluginInstance(handler, p, variantData);
            //QJson::QObjectHelper::qvariant2qobject(variantData, mi);
        }

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

MyInterface * ClientHandler::createPluginInstance(QString pluginName, QMap<QString, QPluginLoader *>& p, QVariantMap map)
{
//get the plugin instance by handlerName
    QObject *plugin = p[pluginName]->instance();

//convert qvariantmap into object
    QJson::QObjectHelper::qvariant2qobject(map, plugin);

//convert object to json
    QVariantMap variantMap = QJson::QObjectHelper::qobject2qvariant(plugin);
    QJson::Serializer serializer;
    QByteArray json = serializer.serialize(variantMap);


    qDebug() << "JOSN-STRING: " << json;


    MyInterface *mi = qobject_cast<MyInterface *>(plugin);

    return mi;
}

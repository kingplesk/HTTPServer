
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
    requests_(0),
    uuid()
{
    lastUpdated = QDateTime::currentDateTime();
    i_ = 0;
}

void ClientHandler::sendComet(QString json)
{
    QMutableMapIterator<Http *, QTimer *> i(comets_);
    while (i.hasNext()) {
        i.next();

        if (i.key()->state() != QAbstractSocket::ConnectedState) {
            i.remove();
            continue;
        }

        //qDebug() << "isTimerActive : " << i.value()->isActive() << i.value();

        if (i.value()->isActive()) {
            i.value()->stop();
            //i.value()->deleteLater();

            //qDebug() << "stopTimer";
        }

        QAbstractSocket::SocketState state = i.key()->state();
        //qDebug() << "QMutableVectorIterator : " << state;
        if (state == QAbstractSocket::ConnectedState) {
            //qDebug() << "   next->sendReply : " << state;
            lastUpdated = QDateTime::currentDateTime();
            i.key()->sendReply(QByteArray().append(json));
        }
        else {
            //qDebug() << "   i.remove : " << state;
            //timer_

        }

        i.remove();
    }

    checkState();
}

void ClientHandler::checkState()
{
    QDateTime now = QDateTime::currentDateTime();
    if (comets_.isEmpty() && lastUpdated.secsTo(now) > (2 * 60)) {
        deleteLater();

        emit deleteClientHandler(uuid);
    }
}

void ClientHandler::newComet(Http * http, QMap<QString, QPluginLoader *>& p)
{
    QTimer * timer = new QTimer(http);

    connect(timer, SIGNAL(timeout()), this, SLOT(closeComet()));

    timer->setSingleShot(true);
    timer->setInterval(30 * 1000);
    timer->start();

    lastUpdated = QDateTime::currentDateTime();

    comets_[http] = timer;
}

void ClientHandler::closeComet()
{
    QTimer * timer = qobject_cast<QTimer *>(sender());
    Http * http = comets_.key(timer);
    http->closeComet();
    comets_.remove(http);

    checkState();
}

void ClientHandler::newRequest(Http * http, QMap<QString, QPluginLoader *>& p)
{
    //requests_.append(http);

    lastUpdated = QDateTime::currentDateTime();

    QByteArray byteArray = QByteArray().append(http->request_->getBody());
    QString json = QUrl::fromEncoded(byteArray).toString();
    //qDebug() << json;
/*
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
*/





    QByteArray qjson = QByteArray().append(json);
    QJson::Parser parser;
    bool ok;
    // json is a QString containing the data to convert
    QVariantMap variantMap = parser.parse (qjson, &ok).toMap();
    if (!ok) {
        qFatal("An error occurred during parsing");
        exit (1);
    }

    QString handler;
    if (variantMap.contains("handler")) {
        handler = variantMap["handler"].toString();
    }

    //qDebug() << variantMap;

    QVariantMap variantData = variantMap["data"].toMap();

    //qDebug() << variantData;




    QString reply("");
    if (p.contains(handler)) {
        MyInterface *mi = createPluginInstance(handler, p);


        QObject *object = dynamic_cast<QObject *>(mi);
        //convert qvariantmap into object
        QJson::QObjectHelper::qvariant2qobject(variantData, object);
        //convert object to json
        QVariantMap newVariantMap = QJson::QObjectHelper::qobject2qvariant(object);
        QJson::Serializer serializer;
        QByteArray json = serializer.serialize(newVariantMap);
        //qDebug() << "JOSN-STRING-TEST-1: " << json;

        //Server * parentServer = qobject_cast<Server *>(this->parent());
        //parentServer->broadcast(QString().append(json));

        emit broadcast(QString().append("[0, {\"handler\": \"" + handler + "\", \"data\": " + QString().append(json) + "}]"));

        //qDebug() << "newRequest";
/*
        if (handler.contains("paint")) {
            MyInterface *myinstance = createPluginInstance(handler, p, variantData);
            //QJson::QObjectHelper::qvariant2qobject(variantData, mi);
        }
*/

        reply.append("[0, {\"handler\":\"" + handler + "\"}]");
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


    //qDebug() << "JOSN-STRING-TEST-2: " << json;


    MyInterface *mi = qobject_cast<MyInterface *>(plugin);

    return mi;
}

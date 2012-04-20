
#include <QUrl>
#include <QStringList>
#include <QScriptValue>
#include <QScriptEngine>
#include <QScriptValueIterator>
#include <QRegExp>
#include <QDateTime>
#include <QVariant>

#include <QJson/Parser>

#include "clienthandler.h"

ClientHandler::ClientHandler(QObject * parent) :
    QObject(parent),
    comets_(0),
    requests_(0),
    test_(0)
{
    i_ = 0;
    test_ = new Test(this);
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





    QByteArray qjson = QByteArray().append(json);
    QJson::Parser parser;
    bool ok;
    // json is a QString containing the data to convert
    QVariant result = parser.parse (qjson, &ok);

    qDebug() << result;







    QString handler;

    /*
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

/*
    const QMetaObject* metaObject = test_->metaObject();
    QStringList properties;
    for(int i = metaObject->propertyOffset(); i < metaObject->propertyCount(); ++i) {
        QString property = QString::fromLatin1(metaObject->property(i).name());
        QScriptValue prop = sc.property(property);

        properties << property;
    }

    qDebug() << " -------- PROPS: " << properties << metaObject->propertyCount();
/*

    QScriptValueIterator it(sc);
    while (it.hasNext()) {
        it.next();

        QString name =  it.name();

        if (it.value().isString()) {        QString newValue_ = it.value().toString(); }
        else if (it.value().isNumber()) {   qsreal newValue_ = it.value().toNumber(); }
        else if (it.value().isBool()) {     bool newValue_ = it.value().toBool(); }
        else if (it.value().isDate()) {     QDateTime newValue_ = it.value().toDateTime(); }
        else if (it.value().isRegExp()) {   QRegExp newValue_ = it.value().toRegExp(); }
        //else if (it.value().isObject()) {   QObject newValue_ = it.value().toObject(); }
        else if (it.value().isNull()) {}
        else if (it.value().isUndefined()) {}
        else if (it.value().isArray()) {}
        else if (it.value().isError()){}
        else if (it.value().isFunction()) {}

        qDebug() << name << newValue_;

        if (it.name() == "handler") {
            handler = it.value().toString();
        }

        qDebug() << it.name() << ": " << it.value().toString();
    }
*/

    QScriptValueIterator its(sc);
    while (its.hasNext()) {
        its.next();

        if (its.name() == "handler") {
            handler = its.value().toString();
        }

        qDebug() << its.name() << ": " << its.value().toString();
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


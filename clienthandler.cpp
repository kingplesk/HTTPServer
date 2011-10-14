#include <QDebug>
#include <QScriptEngine>
#include <QScriptValueIterator>
#include <QUrl>

#include "clienthandler.h"

ClientHandler::ClientHandler(QObject *parent) :
    QObject(parent)
{
}

QByteArray ClientHandler::getHandler(QByteArray snippet)
{
    QByteArray result;
    result = snippet;
    QByteArray handler;

    int pos = result.indexOf("=");
    if (pos != -1) {
        handler = result.left(pos+1);
        result.remove(0, handler.length());
    }

    qDebug() << "getHandler";
    qDebug() << handler.left(handler.length()-1);
    //qDebug() << result;

    //QUrl * url = new QUrl();
    QString json = QUrl::fromEncoded(result).toString();
    //result = url->fromEncoded(result);

    qDebug() << json;

    QScriptValue sc;
    QScriptEngine engine;
    sc = engine.evaluate("(" + QString(json) + ")");// In new versions it may need to look like engine.evaluate("(" + QString(result) + ")");

    qDebug() << sc.toString();

    QObject * qobj = sc.toQObject();
    //fsObj = static_cast<Tptr>(qobj);

    qDebug() << qobj;

    if (sc.property("result").isArray())
    {
             QScriptValueIterator it(sc.property("result"));
             while (it.hasNext()) {
                 it.next();
                 qDebug("Nick %s",it.value().property("nick").toString().toStdString().c_str());
             }
    }

    return snippet;
}

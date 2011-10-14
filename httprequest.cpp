#include "qdebug.h"

#include "httprequest.h"
#include "clienthandler.h"

HTTPRequest::HTTPRequest(QObject * parent) :
    QObject(parent), clientHandler_()
{
}

void HTTPRequest::requestReady(QByteArray snippet)
{
    qDebug() << "newRequest:";

    QByteArray handler = clientHandler_.getHandler(snippet);

    qDebug() << handler;

    emit newRequest(snippet);
}

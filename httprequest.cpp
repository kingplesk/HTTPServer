#include "qdebug.h"

#include "httprequest.h"

HTTPRequest::HTTPRequest(QObject * parent) :
    QObject(parent)
{
}

void HTTPRequest::requestReady(QByteArray snippet)
{
    qDebug() << "newRequest:";

    emit newRequest(snippet);
}

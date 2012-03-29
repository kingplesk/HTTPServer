#include "qdebug.h"

#include "httprequest.h"
#include "clienthandler.h"

HttpRequest::HttpRequest(QObject * parent) :
    QObject(parent), clientHandler_()
{
}

void HttpRequest::requestReady(QHttpRequestHeader header, QByteArray body)
{
    qDebug() << "newRequest:" << header.path();

    if (header.path().contains("notify", Qt::CaseInsensitive)) {
        comet_ = new Comet();
        //connect(&request_, SIGNAL(newRequest(QHttpRequestHeader, QByteArray)), &response_, SLOT(newResponse(QHttpRequestHeader, QByteArray)));
        //connect(comet_, SIGNAL(timeout()), )
    }
    else {
        QByteArray handler = clientHandler_.getHandler(body);

        qDebug() << handler;

        emit newRequest(header, body);
    }
}

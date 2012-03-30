#include "qdebug.h"

#include "httprequest.h"

HttpRequest::HttpRequest(QHttpRequestHeader header, QByteArray body, QObject * parent) :
    header_(header), body_(body), QObject(parent)
{
}

bool HttpRequest::isComet()
{
    return header_.path().contains("notify", Qt::CaseInsensitive);
}

bool HttpRequest::isAjax()
{
    return header_.path().contains("ajax", Qt::CaseInsensitive);
}


/*
void HttpRequest::requestReady()
{
    qDebug() << "newRequest:" << header.path();

    if () {
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
*/

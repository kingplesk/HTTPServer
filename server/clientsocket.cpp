#include "httprequest.h"
#include "httpresponse.h"
#include "httpparser.h"
#include "clientsocket.h"

ClientSocket::ClientSocket(QObject * parent, HttpRequest * request, HttpResponse * response, HttpParser * parser) :
    QTcpSocket(parent), request_(request), response_(response), parser_(parser)
{
    parser->parseNext(this);

    connect(response_, SIGNAL(send(QByteArray)), this, SLOT(send(QByteArray)));

    //connect(this, SIGNAL(readyRead()), this, SLOT(readClient()));
    connect(this, SIGNAL(readyRead()), parser, SLOT(parseRequestData()));
    connect(parser, SIGNAL(parserReady(QHttpRequestHeader,QByteArray)), this, SLOT(readClient(QHttpRequestHeader, QByteArray)));
    connect(this, SIGNAL(disconnected()), this, SLOT(deleteLater()));
}

void ClientSocket::readClient(QHttpRequestHeader header, QByteArray body)
{
    request_->requestReady(header, body);
}

void ClientSocket::send(QByteArray response)
{
    qDebug() << "send:";

    write(response);

    disconnect(response_, 0, 0, 0);

    disconnectFromHost();
}

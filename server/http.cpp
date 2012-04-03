
#include "http.h"
#include "clienthandler.h"

Http::Http(QTcpSocket * socket, QObject * parent) :
    QObject(parent),
    socket_(socket)
{
    connect(socket_, SIGNAL(disconnected()), socket_, SLOT(deleteLater()));
    connect(socket_, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(socketError(QAbstractSocket::SocketError)));
    connect(socket_, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(socketStateChanged(QAbstractSocket::SocketState)));

    parser_ = new HttpParser(this);
}

void Http::parse()
{
    connect(parser_, SIGNAL(parserReady(QHttpRequestHeader, QByteArray)), this, SLOT(parserReady(QHttpRequestHeader, QByteArray)));
    parser_->parseNext(socket_);
}

void Http::parserReady(QHttpRequestHeader header, QByteArray body)
{
    disconnect(parser_, SIGNAL(parserReady(QHttpRequestHeader, QByteArray)));

    request_ = new HttpRequest(header, body, this);
    response_ = new HttpResponse(this);

    emit newRequest();
}

void Http::socketError(QAbstractSocket::SocketError socketError)
{
    //qDebug() << "socketErrror:" << socketError;
}

void Http::socketStateChanged(QAbstractSocket::SocketState socketState)
{
    //qDebug() << "socketState:" << socketState;
}

void Http::closeComet()
{
    //QTimer * t = qobject_cast<QTimer *>(sender());
    //ClientHandler * ch = (ClientHandler *)t->parent();
    sendReply(QByteArray("Reply Comet: "));
}

void Http::sendReply(QByteArray body)
{
/*
    QByteArray header("HTTP/1.1 200 OK\r\n"
                      "Connection: close\r\n"
                      "Set-Cookie: sid=1;\r\n"
                      "Content-Type: text/html\r\n\r\n");
*/
    socket_->write(response_->getResponse().append(body));
    socket_->close();
}


#include "http.h"
#include "clienthandler.h"

Http::Http(QTcpSocket * socket, QObject * parent) :
    socket_(socket), QObject(parent)
{
    connect(socket_, SIGNAL(disconnected()), socket_, SLOT(deleteLater()));
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
    emit newRequest();
}

void Http::closeComet()
{
    //QTimer * t = qobject_cast<QTimer *>(sender());
    //ClientHandler * ch = (ClientHandler *)t->parent();
    sendReply(QByteArray("Reply Comet: "));
}

void Http::sendReply(QByteArray body)
{

    QByteArray header("HTTP/1.1 200 OK\r\n"
                      "Connection: close\r\n"
                      "Set-Cookie: sid=1;\r\n"
                      "Content-Type: text/html\r\n\r\n");

    socket_->write(header.append(body));
    socket_->close();
}


#include "http.h"

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

void Http::newComet()
{
    timer_ = new QTimer(this);
    connect(timer_, SIGNAL(timeout()), this, SLOT(closeComet()));
    timer_->setSingleShot(true);
    timer_->start(30 * 1000);
}

void Http::closeComet()
{
    sendReply();
}

void Http::sendReply()
{

    socket_->write("HTTP/1.1 200 OK\r\n"
          "Connection: close\r\n"
          "Content-Type: text/html\r\n\r\n"
          "CONTENT");
    socket_->close();
}

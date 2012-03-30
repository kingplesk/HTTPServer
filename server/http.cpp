
#include "http.h"

Http::Http(QObject * parent) :
    QTcpSocket(parent)
{
    connect(this, SIGNAL(disconnected()), this, SLOT(deleteLater()));
    parser_ = new HttpParser(this);
}

void Http::parse()
{
    connect(parser_, SIGNAL(parserReady()), this, SLOT(parserReady()));
    parser_->parseNext(this);
}

void Http::parserReady()
{
    disconnect(parser_, SIGNAL(parserReady()));
    emit newRequest();
}

void Http::newComet()
{
    timer_ = new QTimer(this);
    timer_->setSingleShot(true);
    timer_->start(30 * 1000);
}

void Http::sendReply()
{

    write("HTTP/1.1 200 OK\r\n"
          "Connection: close\r\n"
          "Content-Type: text/html\r\n\r\n"
          "CONTENT");
    close();
}

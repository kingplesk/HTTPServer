
#include "clienthandler.h"

ClientHandler::ClientHandler(QObject * parent) :
    QObject(parent)
{
    i_ = 0;
}

void ClientHandler::newComet(Http * http)
{
    timer_ = new QTimer(this);
    timer_->singleShot(30 * 1000, http, SLOT(closeComet()));
}

void ClientHandler::newRequest(Http * http)
{
    http->sendReply(QByteArray("Reply Ajax: ").append((char)i_));
}

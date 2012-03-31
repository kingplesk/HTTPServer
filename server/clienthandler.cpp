
#include "clienthandler.h"

ClientHandler::ClientHandler(QObject *parent) :
    QObject(parent)
{
    i_ = 0;
}

void ClientHandler::newComet()
{
    timer_ = new QTimer(this);
    timer_->singleShot(30 * 1000, http_, SLOT(closeComet()));
    //parent()->server_->addPendingConnection(http_->socket_);
}

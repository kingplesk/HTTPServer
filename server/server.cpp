#include "server.h"
#include "http.h"

Server::Server(QObject * parent) : QTcpServer(parent)
{
    i_ = 0;
}

void Server::start(qint16 port)
{
    if( !listen(QHostAddress::Any, port) ) {
        qCritical("Cannot listen to Port.");
    }
}

void Server::incomingConnection( int descriptor )
{
    Http * http = new Http(this);

    if ( !http->setSocketDescriptor(descriptor) ) {
        qDebug( "Socket error." );
        return;
    }

    this->connect(http, SIGNAL(newRequest()), SLOT(handle()));
    http->parse();
}

void Server::handle()
{
    Http * http = qobject_cast<Http *>(sender());
    this->disconnect(http, SIGNAL(newRequest()));

    qDebug() << i_++ << http /*header.toString()*/;

    http->sendReply();
    //parser = 0;
}

void Server::sendReply(QByteArray response)
{
    //qDebug() << "send:";

    //connect(socket_, SIGNAL(disconnected()), socket_, SLOT(deleteLater()));

    //socket_->write("response");
    //disconnect(response_, 0, 0, 0);
    //socket_->disconnectFromHost();
}


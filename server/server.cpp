
#include "http.h"
#include "server.h"

Server::Server(QObject * parent) : QTcpServer(parent)
{
    i_ = 0;
}

void Server::start(qint16 port)
{
    if( !listen(QHostAddress::Any, port) ) {
        qCritical("Cannot listen to Port.");
    }

    connect(this, SIGNAL(newConnection()), this, SLOT(test()));
}

void Server::test()
{
    qDebug() << "test";
    QTcpSocket * socket = this->nextPendingConnection();
    while (socket) {
        Http * http = new Http(socket, this);
        qDebug() << i_ << "START incoming";
        this->connect(http, SIGNAL(newRequest()), SLOT(handle()));
        http->parse();

        socket = this->nextPendingConnection();
        qDebug() << i_ << "END incoming";
    }
}

/*
void Server::incomingConnection( int descriptor )
{
    qDebug() << i_ << "START incoming";

    Http * http = new Http(this);

    if ( !http->setSocketDescriptor(descriptor) ) {
        qDebug( "Socket error." );
        return;
    }

    addPendingConnection(http);

    this->connect(http, SIGNAL(newRequest()), SLOT(handle()));
    http->parse();

    qDebug() << i_ << "END incoming";
}
*/

void Server::handle()
{
    Http * http = qobject_cast<Http *>(sender());
    this->disconnect(http, SIGNAL(newRequest()));

    if (http->request_->isComet()) {
        http->newComet();
    }

    clients_[QString(i_)] = http;

    qDebug() << i_++ << http->request_ /*header.toString()*/;

    //http->sendReply();
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


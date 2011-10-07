#include "httprequest.h"
#include "httpresponse.h"
#include "clientsocket.h"

#include "server.h"

Server::Server(QObject * parent) :
    QTcpServer(parent), request_(), response_(), parser_()
{
    connect(&request_, SIGNAL(newRequest(QByteArray)), &response_, SLOT(newResponse(QByteArray)));
}

void Server::start(qint16 port)
{
    if( !listen(QHostAddress::Any, port) ) {
        qCritical("Cannot listen to Port.");
    }
}

void Server::incomingConnection( int descriptor )
{
    qDebug( "Server::incomingConnection" );

    ClientSocket * socket = new ClientSocket(this, &request_, &response_, &parser_);

    if ( !socket->setSocketDescriptor(descriptor) ) {
        qDebug( "Socket error." );
        return;
    }
}

#include "server.h"

Server::Server(QObject * parent) : QTcpServer(parent)//, http_()
{
    i_ = 0;
    //connect(&parser_, SIGNAL(parserReady(QHttpRequestHeader, QByteArray)), this, SLOT(newRequest(QHttpRequestHeader, QByteArray)));
    //connect(&http_, SIGNAL(newRequest()), this, SLOT(handle()));
}

void Server::start(qint16 port)
{
    if( !listen(QHostAddress::Any, port) ) {
        qCritical("Cannot listen to Port.");
    }
}

void Server::incomingConnection( int descriptor )
{
    //qDebug( "START Server::incomingConnection" );

    HttpParser * parser = new HttpParser(this);
    QTcpSocket * socket = new QTcpSocket(parser);


    //Http * http = new Http(this);

    if ( !socket->setSocketDescriptor(descriptor) ) {
        qDebug( "Socket error." );
        return;
    }

    this->connect(parser, SIGNAL(parserReady()), SLOT(handle()));
    parser->parseNext(socket);

    //Http http_(socket);
    //http_.setSocket(socket);
    //http_.parse();

    //qDebug( "END Server::incomingConnection" );
}

void Server::handle()
{
    HttpParser * parser = qobject_cast<HttpParser *>(sender());
    this->disconnect(parser, SIGNAL(parserReady()));

    qDebug() << i_++ << parser /*http*//*header.toString()*/;

    parser->sendReply();
    parser = 0;
}

void Server::sendReply(QByteArray response)
{
    //qDebug() << "send:";

    //connect(socket_, SIGNAL(disconnected()), socket_, SLOT(deleteLater()));

    //socket_->write("response");
    //disconnect(response_, 0, 0, 0);
    //socket_->disconnectFromHost();
}


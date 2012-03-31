
#include "http.h"
#include "server.h"

Server::Server(QObject * parent) : QObject(parent)
{
    server_ = new QTcpServer(this);
    i_ = 0;
}
/*
Server::~Server()
{
  server.close();
}
*/
void Server::start(qint16 port)
{
    if( !server_->listen(QHostAddress::Any, port) ) {
        qCritical("Cannot listen to Port.");
    }

    qDebug() << "MaxPendingConnections:" << server_->maxPendingConnections();

    connect(server_, SIGNAL(newConnection()), this, SLOT(test()));
}

void Server::test()
{
    QTcpSocket * socket = server_->nextPendingConnection();

    qDebug() << " ---- " << server_->hasPendingConnections();

    while (socket) {
        Http * http = new Http(socket, this);
        qDebug() << i_ << "START incoming";
        this->connect(http, SIGNAL(newRequest()), SLOT(handle()));
        http->parse();

        socket = server_->nextPendingConnection();
        //qDebug() << i_ << "END incoming";
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

    ClientHandler * ch = 0;

    QString sid = http->request_->getCookie("sid");
    if (!sid.isEmpty() && clients_.contains(sid)) {
        ch = clients_[sid];
    }
    else {
        ch = new ClientHandler(this);
        clients_[sid] = ch;
    }

    ch->http_ = http;

    if (http->request_->isComet()) {
        ch->newComet();
    }
    else {
        http->sendReply();
    }

    qDebug() << ch->i_++ << http->request_ /*header.toString()*/;

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


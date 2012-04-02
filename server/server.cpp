
#include "http.h"
#include "requesthandler.h"
#include "server.h"

Server::Server(QObject * parent) : QObject(parent)
{
    server_ = new QTcpServer(this);
    requestHandler_ = new RequestHandler(this);

    i_ = 0;
}

Server::~Server()
{
  server_->close();
}

void Server::start(qint16 port)
{
    if( !server_->listen(QHostAddress::Any, port) ) {
        qCritical("Cannot listen to Port.");
    }

    //connect(server_, SIGNAL(newConnection()), requestHandler_, SLOT(handle()));
    connect(server_, SIGNAL(newConnection()), this, SLOT(test()));
}

void Server::test()
{
    QTcpSocket * socket = server_->nextPendingConnection();
    while (socket) {
        Http * http = new Http(socket, this);
        this->connect(http, SIGNAL(newRequest()), SLOT(handle()));
        http->parse();

        socket = server_->nextPendingConnection();
    }
}

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

    if (http->request_->isComet()) {
        ch->newComet(http);
    }

    if (http->request_->isAjax()) {
        ch->newRequest(http);
    }

    qDebug() << ch->i_++ << http->request_;
}

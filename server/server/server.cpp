
#include <QDir>
#include <QUuid>
#include <QDateTime>
#include <QCoreApplication>
#include <QMutableMapIterator>

#include "http.h"
#include "server.h"
#include "requesthandler.h"

Server::Server(QObject * parent) :
    QObject(parent),
    server_(),
    requestHandler_(),
    clients_()
{
    i_ = 0;

    connect(&timer_, SIGNAL(timeout()), this, SLOT(update()));
    timer_.start(40 * 1000);

    loadPlugins();
}

Server::~Server()
{
  server_.close();
}

void Server::start(qint16 port)
{
    if( !server_.listen(QHostAddress::Any, port) ) {
        qCritical("Cannot listen to Port.");
    }

    //connect(&server_, SIGNAL(newConnection()), requestHandler_, SLOT(handle()));
    connect(&server_, SIGNAL(newConnection()), this, SLOT(newConnection()));
}

void Server::newConnection()
{
    QTcpSocket * socket = server_.nextPendingConnection();
    while (socket) {
        Http * http = new Http(socket, this);
        this->connect(http, SIGNAL(newRequest()), SLOT(handle()));
        http->parse();

        socket = server_.nextPendingConnection();
    }
}

void Server::handle()
{
    QDateTime before = QDateTime::currentDateTime();

    Http * http = qobject_cast<Http *>(sender());
    this->disconnect(http, SIGNAL(newRequest()));

    ClientHandler * ch = 0;

    QString uuid = http->request_->getCookie("sid");
    if (!uuid.isEmpty() && clients_.contains(uuid)) {
        ch = clients_[uuid];
    }
    else {
        do {
            uuid = QUuid::createUuid().toString();
        } while(clients_.contains(uuid));

        http->response_->addCookie(uuid);

        ch = new ClientHandler(this);
        connect(ch, SIGNAL(broadcast(QString)), this, SLOT(broadcast(QString)));
        clients_[uuid] = ch;
    }

    if (http->request_->isComet()) {
        ch->newComet(http, p_);
    }

    //qDebug() << "is Ajax" << http->request_->isAjax();

    if (http->request_->isAjax()) {
        ch->newRequest(http, p_);

        QDateTime after = QDateTime::currentDateTime();
        qDebug("handle: %d", before.msecsTo(after));
    }

    //qDebug() << http->request_ << http->response_;
}

void Server::update()
{
    broadcast("[0,{\"data\":\"huhu\"}]");
}

void Server::broadcast(QString json)
{
    qDebug() << json;


    QDateTime before = QDateTime::currentDateTime();

    QMutableMapIterator<QString, ClientHandler *> i(clients_);
    while (i.hasNext()) {
        i.next();
        i.value()->sendComet(json);
    }

    QDateTime after = QDateTime::currentDateTime();
    qDebug("broadcast: %d", before.msecsTo(after));
}


void Server::loadPlugins()
{
    pluginsDir_ = QDir(QCoreApplication::applicationDirPath());

#if defined(Q_OS_WIN)
    if (pluginsDir_.dirName().toLower() == "debug" || pluginsDir_.dirName().toLower() == "release")
        pluginsDir_.cdUp();
#elif defined(Q_OS_MAC)
    if (pluginsDir_.dirName() == "MacOS") {
        pluginsDir_.cdUp();
        pluginsDir_.cdUp();
        pluginsDir_.cdUp();
    }
#endif
    pluginsDir_.cd("plugins");

    //load plugins and store loaded plugins in qmap for later use of new instances
    foreach (QString fileName, pluginsDir_.entryList(QDir::Files)) {
        QPluginLoader * l = new QPluginLoader(pluginsDir_.absoluteFilePath(fileName));
        l->load();
        if (l->isLoaded()) {
            QString pluginName(fileName);
            QRegExp rx("^.*lib_([^.]*).*$", Qt::CaseInsensitive);

            if (rx.exactMatch(fileName)) {
                qDebug() << rx.cap(1);
                pluginName = rx.cap(1);
            }

            p_[pluginName] = l;
        }
    }

    qDebug() << p_;
}

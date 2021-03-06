
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
    timer_.start(2 * 60 * 1000);

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

void Server::deleteClientHandler(QString uuid)
{
    if (clients_.contains(uuid)) {
        qDebug() << "remove ClientHandler : " << uuid;
        clients_.remove(uuid);
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
        while (uuid.isEmpty() && !clients_.contains(uuid)) {
            uuid = QUuid::createUuid().toString();
        }

        http->response_->addCookie(uuid);

        ch = new ClientHandler(this);

        //connect(ch, SIGNAL(broadcast(QString)), this, SLOT(broadcast(QString)));
        //connect(ch, SIGNAL(broadcast(QString,  QString)), this, SLOT(broadcast(QString, QString)));
        connect(ch, SIGNAL(broadcast(QString,  QString, bool)), this, SLOT(broadcast(QString, QString, bool)));
        connect(ch, SIGNAL(deleteClientHandler(QString)), this, SLOT(deleteClientHandler(QString)));

        clients_[uuid] = ch;
        ch->uuid = uuid;
    }

    if (http->request_->isComet()) {
        ch->newComet(http, p_);
    }
    else if (http->request_->isAjax()) {
        ch->newRequest(http, p_);

        //QTimer::singleShot(0, ch, )

        QDateTime after = QDateTime::currentDateTime();
        qDebug("handle: %d", before.msecsTo(after));
    }
    else {
        http->sendReply(QByteArray(""));
    }

    //qDebug() << http->request_ << http->response_;
}

void Server::update()
{
    QDateTime before = QDateTime::currentDateTime();

    QMutableMapIterator<QString, ClientHandler *> i(clients_);
    while (i.hasNext()) {
        i.next();
        i.value()->checkState();
    }

    QDateTime after = QDateTime::currentDateTime();
    qDebug("Server::update() cleanUp: %d", before.msecsTo(after));
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

void Server::broadcast(QString json, QString channel)
{
    qDebug() << "JSON:" << json << "CHANNEL:" << channel;

    QDateTime before = QDateTime::currentDateTime();

    QMutableMapIterator<QString, ClientHandler *> i(clients_);
    while (i.hasNext()) {
        i.next();
        if (i.key() == channel) {
            i.value()->sendComet(json);
        }
    }

    QDateTime after = QDateTime::currentDateTime();
    qDebug("broadcast: %d", before.msecsTo(after));
}

void Server::broadcast(QString json, QString channel, bool isInitial)
{
    qDebug() << "JSON:" << json << "CHANNEL:" << channel;

    QDateTime before = QDateTime::currentDateTime();

    QMutableMapIterator<QString, ClientHandler *> i(clients_);
    while (i.hasNext()) {
        i.next();
        if (i.key() == channel) {
            i.value()->sendComet(json, isInitial);
        }
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

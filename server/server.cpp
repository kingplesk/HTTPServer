
#include <QDir>
#include <QUuid>
#include <QCoreApplication>

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
        clients_[uuid] = ch;
    }

    if (http->request_->isComet()) {
        ch->newComet(http);
    }

    if (http->request_->isAjax()) {
        ch->newRequest(http);
    }

    qDebug() << ch->i_++ << http->request_;
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

    //load plugins and store instances in qmap for later use of same instance
    foreach (QString fileName, pluginsDir_.entryList(QDir::Files)) {
        QPluginLoader loader(pluginsDir_.absoluteFilePath(fileName));

        QObject *plugin = loader.instance();

        if (plugin) {
            pluginFileNames_ += fileName;

            plugins_[fileName] = qobject_cast<MyInterface *>(plugin);

            qDebug()  << fileName;

            if (plugins_[fileName])
                qDebug() << plugins_[fileName]->getString();
        }
    }

    qDebug() << plugins_;


    //load plugins and store loaded plugins in qmap for later use of new instances
    foreach (QString fileName, pluginsDir_.entryList(QDir::Files)) {
        QPluginLoader * l = new QPluginLoader(pluginsDir_.absoluteFilePath(fileName));
        l->load();
        if (l->isLoaded()) {

            fileName.append("_test");
            p_[fileName] = l;

            QObject *plugin = p_[fileName]->instance();

            MyInterface *mi = qobject_cast<MyInterface *>(plugin);

            pluginFileNames_ += fileName;

            qDebug()  << fileName;

            if (mi)
                qDebug() << mi->getString();
        }
    }

    qDebug() << p_;

}

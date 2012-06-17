
#include <QUrl>
#include <QDateTime>
#include <QVariant>
#include <QVariantMap>

#include <QJson/Parser>
#include <QJson/Serializer>
#include <QJson/QObjectHelper>

#include "clienthandler.h"

ClientHandler::ClientHandler(QObject * parent) :
    QObject(parent),
    uuid()
{
    lastUpdated = QDateTime::currentDateTime();
}

void ClientHandler::sendComet(QString json)
{
    messageQueue_ << json;
    int messageId = messageQueue_.lastIndexOf(json);

    /*
        http://msdn.microsoft.com/de-de/library/dd293599.aspx
        http://www.cprogramming.com/c++11/c++11-lambda-closures.html

        []          Capture nothing (or, a scorched earth strategy?)
        [&]         Capture any referenced variable by reference
        [=]         Capture any referenced variable by making a copy
        [=, &foo]   Capture any referenced variable by making a copy, but capture variable foo by reference
        [bar]       Capture bar by making a copy; don't copy anything else
        [this]      Capture the this pointer of the enclosing class
    */

    cometIterator([&, this](comet& nextComet) {
        if (nextComet.http && nextComet.http->state() == QAbstractSocket::ConnectedState) {
            //stop auto closing timer for current comet
            if (nextComet.timer->isActive()) {
                nextComet.timer->stop();
            }

            lastUpdated = QDateTime::currentDateTime();
            nextComet.lastUpdated = QDateTime::currentDateTime();

            //qDebug() << "lastMessageId:" << nextComet.lastMessageId <<  "messageId:" << messageId;

            int idx = nextComet.lastMessageId;
            ++idx;

            //qDebug() << "lastMessageId:" << nextComet.lastMessageId <<  "messageId:" << messageId << "idx:" << idx;

            QStringList messages = messageQueue_.mid(idx);

            //qDebug() << "Messages mid:" << messages;

            nextComet.lastMessageId = messageId;

            //qDebug() << "LastMessagesId:" << nextComet.lastMessageId;

            nextComet.http->sendReply(QByteArray().append("[").append(messages.join(", ")).append("]"));
        }

        nextComet.http = 0;
        nextComet.timer = 0;
        nextComet.tid = '-1';
    });

    qDebug() << uuid << "MessageQueue: " << messageQueue_;

    checkState();
}

void ClientHandler::sendComet(comet& nextComet)
{
    int messageId = messageQueue_.count() - 1;
    if (nextComet.http && nextComet.http->state() == QAbstractSocket::ConnectedState) {
        //stop auto closing timer for current comet
        if (nextComet.timer->isActive()) {
            nextComet.timer->stop();
        }

        lastUpdated = QDateTime::currentDateTime();
        nextComet.lastUpdated = QDateTime::currentDateTime();

        int idx = nextComet.lastMessageId;
        ++idx;

        QStringList messages = messageQueue_.mid(idx);
        nextComet.lastMessageId = messageId;
        nextComet.http->sendReply(QByteArray().append("[").append(messages.join(", ")).append("]"));
    }

    nextComet.http = 0;
    nextComet.timer = 0;
    nextComet.tid = '-1';

    qDebug() << uuid << "MessageQueue: " << messageQueue_;

    checkState();
}


bool ClientHandler::cometsEmpty()
{
    for(QMap<QString, comet>::iterator it = comets_.begin(); it != comets_.end(); ++it) {
        comet& nextComet = it.value();
        if (nextComet.http) {
            return false;
        }
    }

    qDebug() << "CometsEmpty:";

    return true;
}

void ClientHandler::checkState()
{
    QDateTime now = QDateTime::currentDateTime();
    if (cometsEmpty() && lastUpdated.secsTo(now) > (2 * 60)) {
        deleteLater();

        emit deleteClientHandler(uuid);
    }
}

void ClientHandler::newComet(Http * http, QMap<QString, QPluginLoader *>& p)
{
    QTimer * timer = new QTimer(http);

    connect(timer, SIGNAL(timeout()), this, SLOT(closeComet()));

    timer->setSingleShot(true);
    timer->setInterval(30 * 1000);
    timer->start();

    lastUpdated = QDateTime::currentDateTime();

    //comet
    QString tid = http->request_->getTid();

    if (!comets_.contains(tid)) {
        comet newComet;
        newComet.tid = tid;
        newComet.lastMessageId = -1;
        newComet.lastUpdated = QDateTime::currentDateTime();
        newComet.http = 0;
        newComet.timer = 0;

        comets_[tid] = newComet;
    }

    if (comets_[tid].http) {
        comets_[tid].http->deleteLater();
        comets_[tid].http = 0;
    }

    if (comets_[tid].timer) {
        if (comets_[tid].timer->isActive()) comets_[tid].timer->stop();
        comets_[tid].timer->deleteLater();
        comets_[tid].timer = 0;
    }

    comets_[tid].http = http;
    comets_[tid].timer = timer;

    qDebug() << "TID:" << tid;

    if (comets_[tid].lastMessageId == -1 && messageQueue_.count() > 0) {
        sendComet(comets_[tid]);
    }

    qDebug() << uuid << 'Number of concurrent comets: ' << comets_.count();
}

void ClientHandler::closeComet()
{
    QTimer * timer = qobject_cast<QTimer *>(sender());
    comet& closingComet = getComet(timer);

    qDebug() << "closingComet:" << closingComet.http->state();
    QString tid = closingComet.tid;

    qDebug() << uuid << "closeComet" << tid;

    closingComet.http->closeComet();
    closingComet.http = 0;
    closingComet.timer = 0;
    closingComet.tid = '-1';

    checkState();
}

comet& ClientHandler::getComet(QTimer * timer)
{
    for(QMap<QString, comet>::iterator it = comets_.begin(); it != comets_.end(); ++it) {
        comet& nextComet = it.value();
        if (nextComet.timer == timer) {
            return nextComet;
        }
    }
}

comet& ClientHandler::getComet(QString tid)
{
    for(QMap<QString, comet>::iterator it = comets_.begin(); it != comets_.end(); ++it) {
        comet& nextComet = it.value();
        if (nextComet.tid == tid) {
            return nextComet;
        }
    }
}

comet& ClientHandler::getComet(Http * http)
{
    for(QMap<QString, comet>::iterator it = comets_.begin(); it != comets_.end(); ++it) {
        comet& nextComet = it.value();
        if (nextComet.http == http) {
            return nextComet;
        }
    }
}

void ClientHandler::newRequest(Http * http, QMap<QString, QPluginLoader *>& p)
{
    lastUpdated = QDateTime::currentDateTime();

    QByteArray byteArray = QByteArray().append(http->request_->getBody());
    QString json = QUrl::fromEncoded(byteArray).toString();
    //qDebug() << json;

    /**
    * QJSON & OwnPlugin
    */
    QByteArray qjson = QByteArray().append(json);
    QJson::Parser parser;
    bool ok;
    // json is a QString containing the data to convert
    QVariantMap variantMap = parser.parse (qjson, &ok).toMap();
    if (!ok) {
        qFatal("An error occurred during parsing");
        exit (1);
    }

    QString handler;
    if (variantMap.contains("handler")) {
        handler = variantMap["handler"].toString();
    }
    QString signal;
    if (variantMap.contains("signal")) {
        signal = variantMap["signal"].toString();
    }

    QVariantMap variantData = variantMap["data"].toMap();

    QString reply("");
    if (p.contains(handler)) {
        MyInterface *mi = createPluginInstance(handler, p);

        QObject *object = dynamic_cast<QObject *>(mi);
        //convert qvariantmap into object
        QJson::QObjectHelper::qvariant2qobject(variantData, object);
        //convert object to json



        //object->process();
        std::string std_signal = signal.toStdString();
        char* cstr = new char [std_signal.size()+1];
        strcpy(cstr, std_signal.c_str());

        QString retVal;
        QMetaObject::invokeMethod(object, cstr, Qt::DirectConnection, Q_RETURN_ARG(QString, retVal));
        qDebug() << retVal;



        QVariantMap newVariantMap = QJson::QObjectHelper::qobject2qvariant(object);
        QJson::Serializer serializer;
        QByteArray json = serializer.serialize(newVariantMap);
        //qDebug() << "JOSN-STRING-TEST-1: " << json;

        emit broadcast(QString().append("[0, {\"handler\": \"" + handler + "\", \"signal\": \"" + signal + "\", \"data\": " + QString().append(json) + "}]"), uuid);

        reply.append("[0, {\"handler\":\"" + handler + "\", \"signal\": \"" + signal + "\",}]");
    }
    else {
        reply.append("[1, {\"handler\":\"nono-plugin\"}]");
    }

    http->sendReply(QByteArray().append(reply));
}

MyInterface * ClientHandler::createPluginInstance(QString pluginName, QMap<QString, QPluginLoader *>& p)
{
    QObject *plugin = p[pluginName]->instance();
    MyInterface *mi = qobject_cast<MyInterface *>(plugin);

    return mi;
}

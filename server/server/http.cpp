
#include "http.h"
#include "clienthandler.h"

Http::Http(QTcpSocket * socket, QObject * parent) :
    QObject(parent),
    socket_(socket),
    request_(0),
    response_(0),
    parser_(0),
    state_(QAbstractSocket::ConnectedState)
{
    //connect(socket_, SIGNAL(disconnected()), this, SLOT(deleteLater()));
    connect(socket_, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(socketError(QAbstractSocket::SocketError)));
    connect(socket_, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(socketStateChanged(QAbstractSocket::SocketState)));

    socket_->setParent(this);
    parser_ = new HttpParser(this);
}

void Http::parse()
{
    connect(parser_, SIGNAL(parserReady(QHttpRequestHeader, QByteArray)), this, SLOT(parserReady(QHttpRequestHeader, QByteArray)));
    parser_->parseNext(socket_);
}

void Http::parserReady(QHttpRequestHeader header, QByteArray body)
{
    disconnect(parser_, SIGNAL(parserReady(QHttpRequestHeader, QByteArray)));

    request_ = new HttpRequest(header, body, this);
    response_ = new HttpResponse(this);

    emit newRequest();
}

void Http::socketError(QAbstractSocket::SocketError socketError)
{
    qDebug() << "socketErrror:" << socketError;
}

QAbstractSocket::SocketState Http::state()
{
    return state_;
}

void Http::socketStateChanged(QAbstractSocket::SocketState socketState)
{
    if (socket_->state() != QAbstractSocket::ConnectedState &&
        socket_->state() != QAbstractSocket::ClosingState) {

        state_ = QAbstractSocket::ClosingState;

        //qDebug() << "stateChanged:" << socket_->state() << ":" << state_;

        deleteLater();
    }
}

void Http::closeComet()
{
    //qDebug() << "sendCloseResponse";

    sendReply(QByteArray(""));
}

void Http::sendReply(QByteArray body)
{
    if (state_ != QAbstractSocket::ConnectedState) {
        //qDebug() << "sendReply Sate:" << socket_->state() << ":" << state_;
    }

    /*if (!response_) {
        qDebug() << "sendReply Sate:" << socket_->state() << ":" << state_;
    }*/
    //qDebug() << "getResponse : " << response_;

    state_ = QAbstractSocket::ClosingState;
    socket_->write(response_->getResponse().append(body));
    socket_->close();
}

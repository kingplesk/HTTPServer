#ifndef HTTP_H
#define HTTP_H

#include <QTimer>
#include <QTcpSocket>

#include "httpparser.h"
#include "httprequest.h"
#include "httpresponse.h"

class Http : public QObject
{
    Q_OBJECT

    public:
        Http(QTcpSocket * socket, QObject * parent = 0);
        void sendReply(QByteArray body);
        void parse();

        HttpRequest * request_;
        HttpResponse * response_;

    signals:
        void newRequest();

    public slots:
        void closeComet();
        void parserReady(QHttpRequestHeader header, QByteArray body);
        void socketError(QAbstractSocket::SocketError);
        void socketStateChanged(QAbstractSocket::SocketState);

    private:
        QTcpSocket * socket_;
        HttpParser * parser_;
};

#endif // HTTP_H

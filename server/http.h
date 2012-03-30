#ifndef HTTP_H
#define HTTP_H

#include <QTimer>
#include <QTcpSocket>

#include "httpparser.h"
#include "httprequest.h"

class Http : public QObject
{
    Q_OBJECT

    public:
        explicit Http(QTcpSocket * socket, QObject * parent = 0);
        void sendReply();
        void newComet();
        void parse();

        HttpRequest * request_;

    signals:
        void newRequest();

    public slots:
        void closeComet();
        void parserReady(QHttpRequestHeader header, QByteArray body);

    private:
        QTcpSocket * socket_;
        HttpParser * parser_;
        QTimer * timer_;
};

#endif // HTTP_H

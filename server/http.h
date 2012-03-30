#ifndef HTTP_H
#define HTTP_H

#include <QTimer>
#include <QTcpSocket>

#include "httpparser.h"
#include "httprequest.h"

class Http : public QTcpSocket
{
    Q_OBJECT

    public:
        explicit Http(QObject * parent = 0);
        void sendReply();
        void newComet();
        void parse();

    signals:
        void newRequest();

    public slots:
        void parserReady();

    private:
        HttpParser * parser_;
        HttpRequest * request_;
        QTimer * timer_;
};

#endif // HTTP_H

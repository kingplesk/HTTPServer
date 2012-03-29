#ifndef HTTP_H
#define HTTP_H

#include <QTcpSocket>
#include "httpparser.h"

class Http : public QTcpSocket
{
    Q_OBJECT

    public:
        explicit Http(QObject * parent = 0);
        void sendReply();
        void parse();

    signals:
        void newRequest();

    public slots:
        void parserReady();

    private:
        HttpParser * parser_;
};

#endif // HTTP_H

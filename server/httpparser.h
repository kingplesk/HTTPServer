#ifndef HTTPPARSER_H
#define HTTPPARSER_H

#include <QTcpSocket>
#include <QHttpRequestHeader>
#include <QObject>
#include "http.h"

class HttpParser : public QObject
{
    Q_OBJECT

    public:
        explicit HttpParser(QObject * parent = 0);
        void parseNext(QTcpSocket * socket = 0);
        void sendReply();
        void ready();

    signals:
        //void parserReady(QHttpRequestHeader, QByteArray);
        void parserReady();

    public slots:
        void parseRequestData();

    private:
        QByteArray data_;
        QTcpSocket * socket_;
        QHttpRequestHeader * header_;
        qint64 bytesRead_;

};

#endif // HTTPPARSER_H

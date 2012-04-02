#ifndef HTTPPARSER_H
#define HTTPPARSER_H

#include <QTcpSocket>
#include <QHttpRequestHeader>
#include <QObject>

class HttpParser : public QObject
{
    Q_OBJECT

    public:
        HttpParser(QObject * parent = 0);
        void parseNext(QTcpSocket * socket = 0);
        void ready();

    signals:
        void parserReady(QHttpRequestHeader, QByteArray);

    public slots:
        void parseRequestData();

    private:
        QTcpSocket * socket_;
        QByteArray data_;
        QHttpRequestHeader header_;
        qint64 bytesRead_;

};

#endif // HTTPPARSER_H

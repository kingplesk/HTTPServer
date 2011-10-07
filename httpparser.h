#ifndef HTTPPARSER_H
#define HTTPPARSER_H

#include <QTcpSocket>
#include <QHttpRequestHeader>
#include <QObject>

class HTTPParser : public QObject
{
    Q_OBJECT

    public:
        explicit HTTPParser(QObject *parent = 0);
        void init(QTcpSocket * socket);

    signals:
        void readyRead(QByteArray snippet);

    public slots:
        void readClient();

    private:
        QByteArray data_;
        QTcpSocket * socket_;
        QHttpRequestHeader * header_;
        qint64 bytesRead_;
        QByteArray body_;

};

#endif // HTTPPARSER_H

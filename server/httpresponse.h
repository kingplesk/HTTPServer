#ifndef HTTPRESPONSE_H
#define HTTPRESPONSE_H


#include <QObject>
#include <QByteArray>
#include <QHttpRequestHeader>

class HttpResponse : public QObject
{
    Q_OBJECT

    public:
        explicit HttpResponse(QObject *parent = 0);

    public slots:
        void newResponse(QHttpRequestHeader header, QByteArray body);

    signals:
        void send(QByteArray response);

};

#endif // HTTPRESPONSE_H

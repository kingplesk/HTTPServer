#ifndef HTTPRESPONSE_H
#define HTTPRESPONSE_H

#include <QByteArray>
#include <QObject>

class HTTPResponse : public QObject
{
    Q_OBJECT

    public:
        explicit HTTPResponse(QObject *parent = 0);

    public slots:
        void newResponse(QByteArray request);

    signals:
        void send(QByteArray response);

};

#endif // HTTPRESPONSE_H

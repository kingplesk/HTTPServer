#ifndef HTTPRESPONSE_H
#define HTTPRESPONSE_H


#include <QObject>
#include <QByteArray>
#include <QHttpRequestHeader>

class HttpResponse : public QObject
{
    Q_OBJECT

    public:
        HttpResponse(QObject * parent = 0);
        void addHeader(QString key, QString value);
        void addCookie(QString cookie);
        void setBody(QByteArray body);
        QByteArray getResponse();

    private:
        QHttpResponseHeader header_;
};

#endif // HTTPRESPONSE_H

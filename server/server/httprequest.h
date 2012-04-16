#ifndef HTTPREQUEST_H
#define HTTPREQUEST_H

#include <QObject>
#include <QByteArray>
#include <QHttpRequestHeader>

class HttpRequest : public QObject
{
    Q_OBJECT

    public:
        HttpRequest(QHttpRequestHeader header, QByteArray body, QObject * parent = 0);
        bool isComet();
        bool isAjax();
        QString getCookie(const QString & name);
        QString getBody();

    private:
        QHttpRequestHeader header_;
        QByteArray body_;
};

#endif // HTTPREQUEST_H

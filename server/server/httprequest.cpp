#include "qdebug.h"

#include "httprequest.h"

HttpRequest::HttpRequest(QHttpRequestHeader header, QByteArray body, QObject * parent) :
    QObject(parent),
    header_(header),
    body_(body)
{
}

QString HttpRequest::getTid()
{
    QRegExp rx("(.*)tid=([^&]*).*");

    if (rx.exactMatch(header_.path())) {
        return rx.cap(2);
    }

    return QString();
}

bool HttpRequest::isComet()
{
    return header_.path().contains("notify", Qt::CaseInsensitive);
}

bool HttpRequest::isAjax()
{
    return header_.path().contains("ajax", Qt::CaseInsensitive);
}

QString HttpRequest::getCookie(const QString & name)
{
    QRegExp rx(QString("(.*[; ])?%1=([^; ]*).*").arg(name));

    foreach (const QString & line, header_.allValues("Cookie")) {
        if (rx.exactMatch(line)) {
            return rx.cap(2);
        }
    }

    return QString();
}

QString HttpRequest::getBody()
{
    return body_;
}

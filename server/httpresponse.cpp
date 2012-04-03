#include "qdebug.h"

#include "httpresponse.h"

HttpResponse::HttpResponse(QObject * parent) :
    QObject(parent),
    header_()
{
    header_.setStatusLine(200, "OK", 1, 1);
    header_.addValue("Connection", "close");
    header_.setContentType("text/html");
}


void HttpResponse::addHeader(QString key, QString value)
{
    header_.addValue(key, value);
}

void HttpResponse::addCookie(QString cookie)
{
    /*
    "Set-Cookie:" Name "=" Wert *(";" Attribut)
    "Cookie:" "$Version" "=" value 1*((";" | ",") Cookie)

    Set-Cookie: letzteSuche="cookie aufbau";
                expires=Tue, 29-Mar-2005 19:30:42 GMT;
                Max-Age=2592000;
                Path=/cgi/suche.py;
                Version="1"
    */

    header_.addValue("Set-Cookie", cookie.prepend("sid="));
}

void HttpResponse::setBody(QByteArray body)
{

}

QByteArray HttpResponse::getResponse()
{
    return QByteArray().append(header_.toString());
}


/*
    QByteArray header("HTTP/1.1 200 OK\r\n"
                      "Connection: close\r\n"
                      "Set-Cookie: sid=1;\r\n"
                      "Content-Type: text/html\r\n\r\n");



void HttpResponse::newResponse(QHttpRequestHeader header, QByteArray body)
{
    qDebug() << "newResponse:";

    QString test_body = "TEST";

    // CR LF -> CRLF -> \r\n
    QByteArray test =  "HTTP/1.1 200 OK\r\n";
               test += "Server: eri.io\r\n";
               test += "Pragma: no-cache\r\n";
               test += "Cache-Control: no-cache, max-age=0, must-revalidate, max-age=86400, public\r\n";
               //test += "Content-Length: ";
               //test.append(body.length());
               //test +=  "\r\n";
               test += "Keep-Alive: timeout=15, max=100\r\n";
               test += "Connection: Keep-Alive\r\n";
               test += "Content-Type: text/html;charset=UTF-8\r\n\r\n";
               test.append( test_body );

   //emit send(test);
}
*/

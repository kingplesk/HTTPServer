#include "qdebug.h"

#include "httpresponse.h"

HttpResponse::HttpResponse(QObject *parent) :
    QObject(parent)
{
}

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

   emit send(test);
}


#ifndef HTTP_P_H
#define HTTP_P_H

#include <QSharedData>

class Http;
class HttpParser;
class QTcpSocket;

class HttpData: public QSharedData {

public:
    HttpData() {}
    HttpData(QTcpSocket * socket, Http * parent) : socket_(socket), parser_(new HttpParser(parent)) {}

    QTcpSocket * socket_;
    HttpParser * parser_;
/*    HttpRequest request_;
    HttpResponse response_;*/
};

#endif // HTTP_P_H

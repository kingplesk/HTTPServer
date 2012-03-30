
#include <QHttpRequestHeader>

#include "httpparser.h"

HttpParser::HttpParser(QObject * parent) :
    QObject(parent)
{    
}

void HttpParser::parseNext(QTcpSocket * socket)
{
    bytesRead_ = 0;
    header_;
    data_ = "";
    socket_ = socket;

    this->connect(socket_, SIGNAL(readyRead()), SLOT(parseRequestData()));
}

void HttpParser::parseRequestData()
{
    int pos = 0;
    qint64 avail = 0;
    QByteArray header = "";

    //qDebug() << "parseRequestData";

    avail = socket_->bytesAvailable();

    if (avail > 0) {
        data_ += socket_->read(avail);

        if (data_.size() > bytesRead_) {
            pos = data_.indexOf("\r\n\r\n");
            if (pos != -1) {
                header = data_.left(pos+4);
            }
            else {
                pos = data_.indexOf("\n\n");
                if (pos != -1) {
                    header = data_.replace(pos, 2, "\r\n\r\n").left(pos+4);
                }
            }
        }

        bytesRead_ += avail;

        int headerSize = header.size();
        if (headerSize > 0) {
            header_ = new QHttpRequestHeader((QString)header);
            data_.remove(0, headerSize);
            //qDebug() << "Header: " + header_->toString();
        }

        bool headerRead = data_.size() < bytesRead_;
        if ( headerRead && !header_->hasContentLength()) {
            //qDebug() << "Body: 0:" + data_;
            //emit parserReady(* header_, data_);
            ready();
        }
        else {
            if (headerRead && header_->value("content-length").toInt() <= data_.size()) {
                //qDebug() << "Body: >0" + data_;
                //emit parserReady(* header_, data_);
                ready();
            }
        }
    }
}

void HttpParser::ready()
{
    this->disconnect(socket_, SIGNAL(readyRead()));
    emit parserReady();
}

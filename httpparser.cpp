#include <QHttpRequestHeader>

#include "httpparser.h"

HTTPParser::HTTPParser(QObject * parent) :
    QObject(parent)
{    
}

void HTTPParser::init(QTcpSocket * socket)
{
    socket_ = socket;

    bytesRead_ = 0;
    header_;
    body_ = "";
    data_ = "";
}

void HTTPParser::readClient()
{
    qint64 avail = 0;
    int pos = 0;
    QByteArray header = "";

    qDebug() << "readClient";

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

        qDebug() << "AVAIL: " + avail;

        int headerSize = header.size();
        if (headerSize > 0) {
            header_ = new QHttpRequestHeader((QString)header);

            data_.remove(0, headerSize);

            qDebug() << "HEADER 0: " + data_;
            qDebug() << "HEADER 1: " + header;
            qDebug() << "HEADER 2: " + (QString)header;
            qDebug() << "HEADER 3: " + header_->toString();
        }

        bool headerRead = data_.size() < bytesRead_;
        if ( headerRead && !header_->hasContentLength()) {
            emit readyRead(data_);
        }
        else {
            if (headerRead && header_->value("content-length").toInt() <= data_.size()) {
                body_ = data_;

                qDebug() << "BODY 0: " + body_;

                emit readyRead(data_);
            }
        }


    }
}

//void HTTPParser::

#ifndef CLIENTHANDLER_H
#define CLIENTHANDLER_H

#include <QObject>
#include <QTimer>
#include <QByteArray>

#include "http.h"

class ClientHandler : public QObject
{
    Q_OBJECT

    public:
        ClientHandler(QObject * parent = 0);
        void newComet(Http * http);
        void newRequest(Http * http);

        qint16 i_;
        Http * http_;

    signals:

    public slots:

    private:
        QTimer * timer_;

};

#endif // CLIENTHANDLER_H

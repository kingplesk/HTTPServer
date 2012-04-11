#ifndef SERVERTHREAD_H
#define SERVERTHREAD_H

#include <QThread>
//#include <QTcpSocket>
//#include <QByteArray>

class ServerThread : public QThread
{
    public:
        ServerThread(/*int descriptor,*/ QObject * parent);
        void run();
        //void sendReply();
        //void close();
        //void check(QByteArray snippet);

    private:
        //int m_descriptor;
        //qint64 avail;
        //QTcpSocket socket;
        //QByteArray request;

    //private slots:
      //  void readRequest();

};

#endif // SERVERTHREAD_H

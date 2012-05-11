#include <QtCore/QCoreApplication>
#include <stdlib.h>

#include "server.h"

using namespace std;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Server server;
    server.start();

    return a.exec();
}

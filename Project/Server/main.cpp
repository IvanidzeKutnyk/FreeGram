#include <QCoreApplication>
#include "cosm_serv.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    c_server server;
    server.startServer();

    return a.exec();
}

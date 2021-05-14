#ifndef COSM_SERV_H
#define COSM_SERV_H

#include "QTcpServer"
#include "QTcpSocket"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QFile>
#include <QJsonValue>
#include <QJsonArray>

class c_server: public QTcpServer
{
    Q_OBJECT
public:
    c_server();
    ~c_server();

    QTcpSocket* socket;
    QByteArray data;

    QJsonDocument doc;
    QJsonParseError docError;

    QSqlDatabase db;
public slots:
    void startServer();
    void incomingConnection(qintptr socketDescriptor);
    void sockReady();
    void sockDisc();
};
#endif // MYSERVER_H

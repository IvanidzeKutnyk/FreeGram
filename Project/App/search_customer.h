#ifndef SEARCH_CUSTOMER_H
#define SEARCH_CUSTOMER_H

#include <QDialog>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QStandardItemModel>
#include <QJsonArray>
#include <QTcpSocket>
#include <QMessageBox>
#include <QDebug>
namespace Ui {
class search_customer;
}

class search_customer : public QDialog
{
    Q_OBJECT

public:
    explicit search_customer(QWidget *parent = nullptr);
    ~search_customer();
    QTcpSocket* socket;
  QByteArray data;

  QJsonDocument doc;
  QJsonParseError docError;
public slots:
    void sockReady();
    void sockDisc();
private slots:
    void on_Clear_B_clicked();

    void on_Exit_B_clicked();

    void on_search_B_clicked();

private:
    Ui::search_customer *ui;
};

#endif // SEARCH_CUSTOMER_H

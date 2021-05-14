#ifndef REGISTRATION_H
#define REGISTRATION_H

#include <QDialog>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QStandardItemModel>
#include <QJsonArray>
#include <QTcpSocket>

namespace Ui {
class registration;
}

class registration : public QDialog
{
    Q_OBJECT

public:
    explicit registration(QWidget *parent = nullptr);
    ~registration();
    QTcpSocket* socket;
     QByteArray data;

  QJsonDocument doc;
  QJsonParseError docError;

public slots:
    void sockReady();
    void sockDisc();
    void on_reg_in_clicked();

    void on_pushButton_back_clicked();

private:
    Ui::registration *ui;
};

#endif // REGISTRATION_H

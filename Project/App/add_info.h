#ifndef ADD_INFO_H
#define ADD_INFO_H

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
class add_info;
}

class add_info : public QDialog
{
    Q_OBJECT

public:
    explicit add_info(QWidget *parent = nullptr);
    ~add_info();
    QTcpSocket* socket;
    QByteArray data;

    QJsonDocument doc;
    QJsonParseError docError;
public slots:
    void sockReady();
    void sockDisc();

private slots:
    void on_Red_clicked();

    void on_Exit_B_clicked();

private:
    Ui::add_info *ui;
};

#endif // ADD_INFO_H

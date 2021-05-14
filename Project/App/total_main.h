#ifndef TOTAL_MAIN_H
#define TOTAL_MAIN_H

#include <QDialog>
#include "ui_total_main.h"
#include "search_customer.h"
#include "exit.h"
#include "QString"
#include "QMessageBox"
#include "QCloseEvent"
#include "add_info.h"
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QStandardItemModel>
#include <QJsonArray>
#include <QTcpSocket>
#include <QDebug>

namespace Ui {
class total_main;
}

class total_main : public QDialog
{
    Q_OBJECT

public:
    explicit total_main(QWidget *parent = nullptr);
    ~total_main();
    QTcpSocket* socket;
    QByteArray data;

    QJsonDocument doc;
    QJsonParseError docError;
    ///
    /// \brief f_gname
    ///Ім'я головного користувача
    QString f_gname;
    ///
    /// \brief s_gname
    ///Прізвище головного користувача
    QString s_gname;
    ///
    /// \brief f_dest_name
    ///Ім'я користувача-отримувача
    QString f_dest_name;
    ///
    /// \brief s_dest_name
    ///Прізвище користувача-отримувача
    QString s_dest_name;
    ///
    /// \brief text
    ///Текст повідомлення
    QString text;
    ///
    /// \brief check_name
    ///Резервне ім'я користувача-отримувача
    QString check_name;
    ///
    /// \brief check_sname
    ///Резервне Прізвище користувача-отримувача

    int ABC = 1;
    QString getText();
public slots:
    void sockReady();
    void sockDisc();

private slots:

    void on_Clear_b_clicked();

    void on_Send_b_clicked();

    void on_Search_b_clicked();

    void on_EXIT_B_clicked();


    void on_Connect_B_clicked();

    void on_Select_Cust_clicked();

    void on_refresh_B_clicked();

    void on_ID_clicked();

private:
    Ui::total_main *ui;
    void closeEvent(QCloseEvent *event);
};

#endif // TOTAL_MAIN_H

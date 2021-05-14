#include "registration.h"
#include "ui_registration.h"
#include "QMessageBox"
#include "mainwindow.h"
#include "total_main.h"

registration::registration(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::registration)
{
    ui->setupUi(this);
    socket = new QTcpSocket(this);
           connect(socket,SIGNAL(readyRead()),this,SLOT(sockReady()));
           connect(socket,SIGNAL(disconnected()),this,SLOT(sockDisc()));
           socket->connectToHost("127.0.0.1.",5555);


}

registration::~registration()
{
    delete ui;
}
// Кнопка РЕЄСТРАЦІЯ + перевірка реєстрації
void registration::on_reg_in_clicked()
{
    QString login_r = ui->login_r->text();
    QString password_r = ui->pass_r->text();

    QMessageBox *log_info = new QMessageBox();
     log_info->setStyleSheet("background-color:rgb(0, 0, 120); color:white;");

    if(login_r == "TEST" && password_r == "1234")
    {
        log_info->setWindowTitle("Успіх");
        log_info->setText("Успішна реєстрація");
        log_info->open();
       }else {

        log_info->setWindowTitle("Помилка");
        log_info->setText("Помилка реєстрації");
        log_info->open();
         }
}
// Кнопка НАЗАД
void registration::on_pushButton_back_clicked()
{
  hide();
}
void registration::sockDisc()
{
    socket->deleteLater();
}
void registration::sockReady()
{

}

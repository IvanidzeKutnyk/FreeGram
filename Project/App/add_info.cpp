#include "add_info.h"
#include "ui_add_info.h"

add_info::add_info(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::add_info)
{
    ui->setupUi(this);
    socket = new QTcpSocket(this);
           connect(socket,SIGNAL(readyRead()),this,SLOT(sockReady()));
           connect(socket,SIGNAL(disconnected()),this,SLOT(sockDisc()));
           socket->connectToHost("127.0.0.1.",3333);
}

add_info::~add_info()
{
    delete ui;
}
void add_info::sockReady()
{


    QMessageBox *msg_d = new QMessageBox();
    QMessageBox *msg_e = new QMessageBox();
    QMessageBox *msg_f = new QMessageBox();
    QMessageBox *msg_g = new QMessageBox();
    msg_d->setStyleSheet("background-color:rgb(0, 0, 120); color:white;");
    msg_d->setWindowTitle("Інформація");
    msg_e->setStyleSheet("background-color:rgb(0, 0, 120); color:white;");
    msg_e->setWindowTitle("Інформація");
    msg_f->setStyleSheet("background-color:rgb(0, 0, 120); color:white;");
    msg_f->setWindowTitle("Інформація");
    msg_g->setStyleSheet("background-color:rgb(0, 0, 120); color:white;");
    msg_g->setWindowTitle("Інформація");


    if (socket->waitForConnected(1000))
    {
       socket->waitForReadyRead(1000);
       data = socket->readAll();

       doc = QJsonDocument::fromJson(data, &docError);

       if (docError.errorString() == "no error occurred")
       {
           if ((doc.object().value("type").toString() == "connection" && doc.object().value("status").toString() == "y"))
           {
               msg_d->setText("З\'єднано з сервером");
               msg_d->open();
           }

           else if(doc.object().value("REG").toString() == "resultSelect")
           {
               if(doc.object().value("name").toString() == "suc")
               {
                   msg_d->setText("Успішна реєстрація");
                   msg_d->open();
                   ui->stat_info->setText("Зареєстровано!");

               }
               else
               {
                   msg_e->setText("Помилка реєстрації");
                   msg_e->open();
                   ui->stat_info->setText("Помилка!");
               }


           }
           else
           {
               msg_f->setText("Помилка з\'днання з сервером");
               msg_f->open();
           }
       }
       else
       {
           msg_g->setText("Помилки з форматом передачі даних!" + docError.errorString());
           msg_g->open();
       }
    }

}
void add_info::sockDisc()
{
   socket->deleteLater();
}
void add_info::on_Red_clicked()
{
    if(ui->name_in->text() == NULL || ui->sec_name->text() == NULL )
    { QMessageBox::information(this, "INFORMATION", "Ім\'я та Прізвище обов\'зкове для заповнення!");}
    else
    {
    if(socket->isOpen())
       {
        QString str = "{\"REG\":\"INFO\",\"reg_data\":[{\"name\":\"" + ui->name_in->text() + "\",\"sec_name\":\"" + ui->sec_name->text() +
                "\",\"date\":\"" + ui->date->text() + "\",\"nick\":\"" + ui->nick->text() + "\",\"email\":\"" + ui->email->text() +"\"}]}";
        socket->write(str.toStdString().c_str());
        socket->waitForBytesWritten(1000);
        qDebug()<<str;
       }

    }
}

void add_info::on_Exit_B_clicked()
{
    this->close();
}

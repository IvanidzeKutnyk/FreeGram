#include "search_customer.h"
#include "ui_search_customer.h"
#include "QString"

search_customer::search_customer(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::search_customer)
{
    ui->setupUi(this);
    socket = new QTcpSocket(this);
           connect(socket,SIGNAL(readyRead()),this,SLOT(sockReady()));
           connect(socket,SIGNAL(disconnected()),this,SLOT(sockDisc()));
           socket->connectToHost("127.0.0.1.",3333);
}

search_customer::~search_customer()
{
    delete ui;
}

//Кнопка очищення ПОЛІВ
void search_customer::on_Clear_B_clicked()
{
    //Info_In
    ui->name_in->clear();
    ui->second_name_in->clear();
    ui->ID_IN->clear();
    //Info_Out
    ui->name_out->clear();
    ui->second_name_out->clear();
    ui->data_out->clear();
    ui->nik_out->clear();
    ui->email_out->clear();
    ui->ID_out->clear();
    ui->stat_info->clear();
}
//Кнопка закривання ВІКНА
void search_customer::on_Exit_B_clicked()
{
    this->close();
}

void search_customer::on_search_B_clicked()
{
    QMessageBox *msg_ab = new QMessageBox();
    msg_ab->setStyleSheet("background-color:rgb(0, 0, 120); color:white;");
    msg_ab->setWindowTitle("Інформація");
    int id = ui->ID_IN->text().toInt();
    QString stringID;
    stringID = QString::number(id);

    if(socket->isOpen())
       {
           QString str = "{\"CHECK\":\"INFO\",\"check_data\":[{\"id\":\"" + stringID + "\",\"fname\":\"" + ui->name_in->text() + "\",\"sname\":\"" + ui->second_name_in->text() + "\"}]}";
           socket->write(str.toStdString().c_str());
           socket->waitForBytesWritten(1000);
       }
       else
       {
           msg_ab->setText("Невдале З\'єднання з сервером");
           msg_ab->open();
       }

}
void search_customer::sockReady()
{
 QMessageBox *msg_a = new QMessageBox();
 msg_a->setStyleSheet("background-color:rgb(0, 0, 120); color:white;");
 msg_a->setWindowTitle("Інформація");

    if (socket->waitForConnected(1000))
    {
       socket->waitForReadyRead(1000);
       data = socket->readAll();

       doc = QJsonDocument::fromJson(data, &docError);
        qDebug()<<"DATA"<<data;
       if (docError.errorString() == "no error occurred")
       {

           QString a = doc.object().value("fname").toString();
           QString b = ui->name_in->text();
           qDebug()<<"Fname:"<<a<<"  "<<"NAME:"<<b;


            if(doc.object().value("CHECK").toString() == "resultSelect")
           {

                if(doc.object().value("name").toString() == "name_error")
               {
                   ui->name_out->setText("NULL");
                   ui->second_name_out->setText("NULL");
                   ui->data_out->setText("NULL");
                   ui->nik_out->setText("NULL");
                   ui->email_out->setText("NULL");
                   ui->ID_out->setText("NULL");
                   ui->stat_info->setText("Не знайдено!");

               }

               else if((doc.object().value("fname").toString() == ui->name_in->text()) &&(doc.object().value("sname").toString() == ui->second_name_in->text()) )
               {
                   ui->name_out->setText(doc.object().value("fname").toString());
                   ui->second_name_out->setText(doc.object().value("sname").toString());
                   ui->data_out->setText(doc.object().value("Date").toString());
                   ui->nik_out->setText(doc.object().value("Nick").toString());
                   ui->email_out->setText(doc.object().value("Email").toString());
                   ui->ID_out->setText(doc.object().value("ID").toString());
                   ui->stat_info->setText("Знайдено!");

               }




            }


        }
       else
       {
           msg_a->setText("Помилки з форматом передачі даних!" + docError.errorString());
           msg_a->open();
       }
     }
}
void search_customer::sockDisc()
{
    socket->deleteLater();
}

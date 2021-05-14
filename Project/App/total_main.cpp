#include "total_main.h"
#include "ui_total_main.h"


total_main::total_main(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::total_main)
{
    ui->setupUi(this);
    socket = new QTcpSocket(this);
           connect(socket,SIGNAL(readyRead()),this,SLOT(sockReady()));
           connect(socket,SIGNAL(disconnected()),this,SLOT(sockDisc()));

}

total_main::~total_main()
{
    delete ui;
}


//Очищення поля SMS_TEXT
void total_main::on_Clear_b_clicked()
{
    ui->sms_text->clear();
}
//Надсилання смс з поля SMS_TEXT
void total_main::on_Send_b_clicked()
{
    QMessageBox *msg_d = new QMessageBox();
    msg_d->setStyleSheet("background-color:rgb(0, 0, 120); color:white;");
    msg_d->setWindowTitle("Інформація");

    if(f_gname == NULL && s_gname == NULL)
    {
        msg_d->setText("Ідентифікуйтесь!");
        msg_d->open();
    }
    else
    {


    text = ui->sms_text->toPlainText();

        if(socket->isOpen())
        {
            QString str = "{\"SMS\":\"IN\",\"data\":[{\"sms\":\"" + text + "\",\"f_dest_name\":\"" + f_dest_name
                    + "\",\"s_dest_name\":\"" + s_dest_name + "\",\"f_gname\":\"" + f_gname
                    + "\",\"s_gname\":\"" + s_gname +"\"}]}";
            qDebug()<<"-----------";
            qDebug()<<str;
            qDebug()<<"-----------";
            socket->write(str.toStdString().c_str());
            socket->waitForBytesWritten(1000);
        }
        else
        {
            QMessageBox::information(this, "INFORMATION", "Невдале З\'єднання з сервером!");

        }
             ui->sms_text->clear();


}

}
//Виклик вікна пошуку користувача
void total_main::on_Search_b_clicked()
{
  socket->disconnectFromHost();
    search_customer cust;
  cust.setModal(true);
  cust.setMinimumSize(800,480);
  cust.setMaximumSize(800,480);
  cust.setWindowTitle("Searching Customers");
  cust.exec();
}

void total_main::on_EXIT_B_clicked()
{
 this->close();
}
void total_main::closeEvent(QCloseEvent *event)
{
    event->ignore();
        QMessageBox exit;
        exit.setWindowTitle("Бажаєте вийти?");
        exit.setStyleSheet("color:white;background-color:rgb(0, 0, 140);");
        QAbstractButton *yes = exit.addButton("ТАК",QMessageBox::YesRole);
        QAbstractButton *no = exit.addButton("НІ",QMessageBox::NoRole);
        exit.setText("Впевнені?");
        exit.exec();
        if(exit.clickedButton() == yes) event->accept();

}
void total_main::sockDisc()
{

}
void total_main::sockReady()
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
           else if(doc.object().value("SELECT").toString() == "resultSelect")
           {
               QMessageBox *msg_a = new QMessageBox();
               msg_a->setStyleSheet("background-color:rgb(0, 0, 120); color:white;");
               msg_a->setWindowTitle("Інформація");

               if(doc.object().value("status").toString() == "find")
               {
                   QString stI;
                   stI = QString::number(ABC);
                   f_dest_name = ui->name_in->text();
                   s_dest_name = ui->second_name_in->text();
                   ui->ALL_CUST->append(stI + "." + f_dest_name + " " + s_dest_name);
                   ui->SEL_name->setText(f_dest_name);
                   ui->SEL_sec_name->setText(s_dest_name);
                   msg_a->setText("Користувач вибраний");
                   msg_a->open();
                   ABC++;
               }
               else
               {
                   msg_a->setText("Помилка! Користувач не знайдений!");
                   msg_a->open();
               }

           }
           else if(doc.object().value("SMS").toString() == "resultSelect")
           {
               QMessageBox *msg_a = new QMessageBox();
               msg_a->setStyleSheet("background-color:rgb(0, 0, 120); color:white;");
               msg_a->setWindowTitle("Інформація");

               if(doc.object().value("status").toString() == "OK")
               {
                  ui->ALL_LABEL->append("You to "+f_dest_name +" " + s_dest_name+":");
                  ui->ALL_LABEL->append(text);
                  ui->ALL_LABEL->append(">------------------<");
               }
               else
               {
                   msg_a->setText("Помилка! Спробуйте пізніше!");
                   msg_a->open();
               }

           }
           else if(doc.object().value("CHECK_SMS").toString() == "resultSelect")
           {
              if(doc.object().value("status").toString() == "not")
              {
                  ui->ALL_LABEL->append(">------------------<");
                  ui->ALL_LABEL->append("<none>");
                  ui->ALL_LABEL->append(">------------------<");
              }
              else
              {
                  if(f_dest_name == doc.object().value("Dname").toString() && s_dest_name == doc.object().value("Dsname").toString())
                  {
                        ui->ALL_LABEL->append(doc.object().value("sms").toString());
                  }
                  else
                  {

                          f_dest_name = doc.object().value("Dname").toString();
                          s_dest_name = doc.object().value("Dsname").toString();
                          ui->SEL_name->setText(f_dest_name);
                          ui->SEL_sec_name->setText(s_dest_name);
                          ui->ALL_LABEL->append("From  "+f_dest_name +" " + s_dest_name+" to you :");
                            ui->ALL_LABEL->append(doc.object().value("sms").toString());
                          ui->ALL_LABEL->append(">------------------<");

                  }
              }
           }
           else if(doc.object().value("SELMAIN").toString() == "resultSelect")
           {
            if(doc.object().value("status").toString() == "not")
            {
                msg_f->setText("Помилка!");
                msg_f->open();
            }
            else
            {
                f_gname = doc.object().value("Dnname").toString();
                s_gname = doc.object().value("Dsnname").toString();
                msg_f->setText("Ідентифіковано!");
                msg_f->open();
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


void total_main::on_Connect_B_clicked()
{
     socket->connectToHost("127.0.0.1.",3333);
}

void total_main::on_Select_Cust_clicked()
{
    //qDebug()<<"f_gname:"+f_gname;
    //qDebug()<<"s_gname:"+s_gname;
    //qDebug()<<"FiRST:"+ui->name_in->text();
    //qDebug()<<"SECOND:"+ui->second_name_in->text();
    QMessageBox *msg_ab = new QMessageBox();
    msg_ab->setStyleSheet("background-color:rgb(0, 0, 120); color:white;");
    msg_ab->setWindowTitle("Інформація");
    if(f_gname == ui->name_in->text() && s_gname == ui->second_name_in->text())
    {
        msg_ab->setText("Помилка");
        msg_ab->open();
    }
    else
    {

    if(socket->isOpen())
       {
           QString str = "{\"SELECT\":\"CUST\",\"data\":[{\"name\":\"" + ui->name_in->text() + "\",\"sname\":\"" + ui->second_name_in->text() + "\"}]}";
           socket->write(str.toStdString().c_str());
           socket->waitForBytesWritten(1000);
       }
       else
       {
           msg_ab->setText("Невдале З\'єднання з сервером");
           msg_ab->open();
       }
    }
}

void total_main::on_refresh_B_clicked()
{
    QMessageBox *msg_ab = new QMessageBox();
    msg_ab->setStyleSheet("background-color:rgb(0, 0, 120); color:white;");
    msg_ab->setWindowTitle("Інформація");
    if(socket->isOpen())
       {
           QString str = "{\"CHECK\":\"SMS\",\"data\":[{\"name\":\"" + f_gname + "\",\"sname\":\"" + s_gname + "\"}]}";
           socket->write(str.toStdString().c_str());
           socket->waitForBytesWritten(1000);
       }
       else
       {
           msg_ab->setText("Невдале З\'єднання з сервером");
           msg_ab->open();
       }
}

void total_main::on_ID_clicked()
{
    QMessageBox *msg_ab = new QMessageBox();
    msg_ab->setStyleSheet("background-color:rgb(0, 0, 120); color:white;");
    msg_ab->setWindowTitle("Інформація");

  if(socket->isOpen())
      {
          QString str = "{\"CUST\":\"SEL\"}";
          socket->write(str.toStdString().c_str());
          socket->waitForBytesWritten(1000);
      }
      else
      {
       msg_ab->setText("Помилка");
       msg_ab->open();

      }
}

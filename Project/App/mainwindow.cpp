#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "total_main.h"
#include "QMessageBox"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    socket = new QTcpSocket(this);
           connect(socket,SIGNAL(readyRead()),this,SLOT(sockReady()));
           connect(socket,SIGNAL(disconnected()),this,SLOT(sockDisc()));

}

MainWindow::~MainWindow()
{
    delete ui;
}



// Кнопка ВІДОБРАЖЕННЯ інформації про ПО
void MainWindow::on_About_Button_clicked()
{
    QMessageBox *msg_ab = new QMessageBox();
    msg_ab->setStyleSheet("background-color:rgb(0, 0, 120); color:white;");
    msg_ab->setWindowTitle("Інформація");
    msg_ab->setText("Вас вітає FreeGram, v1.0  зборка 0.000030");
    msg_ab->setInformativeText("Робота студента гр. КІ-33, Кутник І.П.");
    msg_ab->open();

}

// Кнопка ВИХІД з початокового меню
void MainWindow::on_EXIT_Button_clicked()
{
    this->close();
}
// Кнопка Авторизація + перевірка авторизації
void MainWindow::on_IN_b_clicked()
{

    //Налаштування Меседж Боксу
    QMessageBox *log_info = new QMessageBox();
     log_info->setStyleSheet("background-color:rgb(0, 0, 120); color:white;");

    //Надсилання запиту на правильність результату
     if(socket->isOpen())
        {
            QString str = "{\"LOG\":\"INFO\",\"login_data\":[{\"log\":\"" + ui->login_l->text() + "\",\"pass\":\"" + ui->pass_l->text() + "\"}]}";
            socket->write(str.toStdString().c_str());
            socket->waitForBytesWritten(1000);
        }
        else
        {
            QMessageBox::information(this, "INFORMATION", "Невдале З\'єднання з сервером!");

        }


}
// Кнопка ВІДКРИТИ вікно рєстрації
void MainWindow::on_Reg_b_clicked()
{
    socket->disconnectFromHost();
    add_info win;
    win.setModal(true);
    win.setMinimumSize(800,480);
    win.setMaximumSize(800,480);
    win.setWindowTitle("Editing Customers");
    win.exec();

}

void MainWindow::on_Connect_B_clicked()
{
    socket->connectToHost("127.0.0.1.",3333);
}
void MainWindow::sockReady()
{

    QMessageBox *msg_a = new QMessageBox();
    QMessageBox *msg_b = new QMessageBox();
    QMessageBox *msg_c = new QMessageBox();
    QMessageBox *msg_d = new QMessageBox();
    QMessageBox *msg_e = new QMessageBox();
    QMessageBox *msg_f = new QMessageBox();
    QMessageBox *msg_g = new QMessageBox();
    msg_a->setStyleSheet("background-color:rgb(0, 0, 120); color:white;");
    msg_a->setWindowTitle("Інформація");
    msg_b->setStyleSheet("background-color:rgb(0, 0, 120); color:white;");
    msg_b->setWindowTitle("Інформація");
    msg_c->setStyleSheet("background-color:rgb(0, 0, 120); color:white;");
    msg_c->setWindowTitle("Інформація");
    msg_d->setStyleSheet("background-color:rgb(0, 0, 120); color:white;");
    msg_d->setWindowTitle("Інформація");
    msg_e->setStyleSheet("background-color:rgb(0, 0, 120); color:white;");
    msg_e->setWindowTitle("Інформація");
    msg_f->setStyleSheet("background-color:rgb(0, 0, 120); color:white;");
    msg_f->setWindowTitle("Інформація");
    msg_g->setStyleSheet("background-color:rgb(0, 0, 120); color:white;");
    msg_g->setWindowTitle("Інформація");

    total_main win_3;
    win_3.setModal(true);
    win_3.setMinimumSize(800,480);
    win_3.setMaximumSize(800,480);
    win_3.setWindowTitle("FreeGram");

    if (socket->waitForConnected(1000))
    {
       socket->waitForReadyRead(1000);
       data = socket->readAll();

       doc = QJsonDocument::fromJson(data, &docError);

       if (docError.errorString() == "no error occurred")
       {
           if ((doc.object().value("type").toString() == "connection" && doc.object().value("status").toString() == "y"))
           {
               msg_a->setText("З\'єднано з сервером");
               msg_a->open();
           }
           else if(doc.object().value("LOG").toString() == "resultSelect")
           {
               if(doc.object().value("name").toString() == ui->login_l->text())
               {
                   QMessageBox* pmbx =
                                       new QMessageBox("Інформація",
                                       "Бажаєте увійти в програму?",
                                       QMessageBox::Information,
                                       QMessageBox::Yes,
                                       QMessageBox::Close,
                                       QMessageBox::No);
                   pmbx->setStyleSheet("background-color:rgb(0, 0, 120); color:white;");
                   int n = pmbx->exec();
                   delete pmbx;

                   if (n == QMessageBox::Yes)
                   {
                     socket->disconnectFromHost();
                     window()->close();
                     win_3.exec();

                   }

               }
               else
               {
                   msg_c->setText("Помилка! Неправильний логін або пароль");
                   msg_c->open();
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
void MainWindow::sockDisc()
{

}

#include "cosm_serv.h"

c_server::c_server(){}

c_server::~c_server(){}

void c_server::startServer()
{
    if (this->listen(QHostAddress::Any,3333))
    {
        db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName("C:\\ССС\\TOTAL_PRO.db");
        if(db.open())
        {
           qDebug() << "Listening and db open";
        }
        else
        {
            qDebug()<<"db open error";
        }

    }
    else
    {
        qDebug() << "Not listening";
    }
}

void c_server::incomingConnection(qintptr socketDescriptor)
{
    socket = new QTcpSocket(this);
    socket->setSocketDescriptor(socketDescriptor);

        connect(socket,SIGNAL(readyRead()),this,SLOT(sockReady()));
        connect(socket,SIGNAL(disconnected()),this,SLOT(sockDisc()));
    qDebug() << "______________________";
    qDebug()<<socketDescriptor<<" Client is connected!";

    socket->write("{\"type\":\"connection\", \"status\":\"y\"}");
    qDebug() << "______________________";
    qDebug()<<"Send client connect status - YES";
    qDebug() << "______________________";
}

void c_server::sockReady()
{
    data = socket->readAll();
    qDebug() << "______________________";
    qDebug() << "Data: " << data;
    qDebug() << "______________________";

    doc = QJsonDocument::fromJson(data, &docError);

    if(docError.errorString() == "no error occurred")
    {
        QJsonValue valuev;
        QJsonArray array = valuev.toArray();
        QJsonValue log;
        QJsonValue pass;
        QSqlQuery * logQ = new QSqlQuery(db);
        QSqlQuery * passQ = new QSqlQuery(db);

        QByteArray summary;
        //LOG_IN
        if((doc.object().value("LOG").toString() == "INFO"))
        {

            QSqlQuery * staQ = new QSqlQuery(db);
            valuev = doc.object().value("login_data");
            array = valuev.toArray();

            foreach (const QJsonValue & v, array)
                        {
                                log = v.toObject().value("log").toString();
                                pass = v.toObject().value("pass").toString();
                        }
            if(db.open())
            {
                summary = "{\"LOG\":\"resultSelect\",";
                QString log1;
                QString pass1;
               if(logQ->exec("SELECT LOG_IN FROM LOG") && passQ->exec("SELECT PASS_IN FROM LOG"))
               {
                   while(logQ->next()&&passQ->next())
                   {
                        if(logQ->value(0).toString() == log.toString() && passQ->value(0).toString() == pass.toString())
                        {
                              summary.append("\"name\":\"" + logQ->value(0).toString().toUtf8() + "\"}");
                        }
                   }
               }
               if(logQ->exec("SELECT LOG_IN FROM LOG")
               && passQ->exec("SELECT PASS_IN FROM LOG"))
               {
                   while(logQ->next() && passQ->next())
                   {
                        if(logQ->value(0).toString() == log.toString() && passQ->value(0).toString() == pass.toString())
                        {
                         staQ->prepare("UPDATE LOG SET Log_status = '1' WHERE LOG_IN = '"+log.toString()+"' and PASS_IN = '"+pass.toString()+"'");
                       if (staQ->exec())
                       {
                           qDebug()<<"--------";
                           qDebug()<<"TOTAL: {\"UPDATE\":\"resultSelect\",\"UPDATE\":\"OK\"}";
                           break;
                       }
                       else
                       {
                           qDebug()<<"--------";
                           qDebug()<< "{\"UPDATE\":\"resultSelect\",\"status\":\"not\"}";
                       }


                        }
                   }
               }


              if(summary == "{\"LOG\":\"resultSelect\",") summary = "{\"LOG\":\"resultSelect\",\"name\":\"name_error\"}";
                               socket->write(summary);
                               socket->waitForBytesWritten(1000);
                               qDebug() << "______________________";
                               qDebug() << summary;
                               qDebug() << "______________________";
            }
            else
            {qDebug() << "______________________";
             qDebug() << "Query error";
             qDebug() << "______________________";
            }


        }
        //REG_IN
        else if((doc.object().value("REG").toString() == "INFO"))
        {
            int  id = 0 ;
            QString stID;
            QJsonValue name;
            QJsonValue sname;
            QJsonValue date;
            QJsonValue nick;
            QJsonValue email;
            QSqlQuery * IDQ = new QSqlQuery(db);
             QSqlQuery * nameQ = new QSqlQuery(db);
              QSqlQuery * snameQ = new QSqlQuery(db);
               QSqlQuery * dateQ = new QSqlQuery(db);
                QSqlQuery * nickQ = new QSqlQuery(db);
                 QSqlQuery * emailQ = new QSqlQuery(db);
                  QSqlQuery * ALL = new QSqlQuery(db);



            valuev = doc.object().value("reg_data");
            array = valuev.toArray();

            foreach (const QJsonValue & v, array)
            {
              name = v.toObject().value("name").toString();
              sname = v.toObject().value("sec_name").toString();
              date = v.toObject().value("date").toString();
              nick = v.toObject().value("nick").toString();
              email = v.toObject().value("email").toString();
            }
            summary = "{\"REG\":\"resultSelect\",";

            if(nameQ->exec("SELECT LOG_IN FROM LOG") && dateQ->exec("SELECT Date FROM LOG") &&
                    snameQ->exec("SELECT PASS_IN FROM LOG") &&  IDQ->exec("SELECT ID FROM LOG") &&
                     nickQ->exec("SELECT Nickname FROM LOG") && emailQ->exec("SELECT Email FROM LOG"))
            {
                while(nameQ->next()&&snameQ->next())
                {
                     if(nameQ->value(0).toString() == name.toString() && snameQ->value(0).toString() == sname.toString())
                     {
                           summary = "{\"REG\":\"resultSelect\",\"name\":\"name_error\"}";
                           break;
                     }
                }
                while(IDQ->next()){id++;}
            }
            id++;
            stID = QString::number(id);
            if(summary != "{\"REG\":\"resultSelect\",\"name\":\"name_error\"}" )
            {
                     ALL->prepare("INSERT INTO LOG(ID,LOG_IN,PASS_IN,Date,Nickname,Email,Log_status,SMS_IN,SMS_IN_FROM_N,SMS_IN_FROM_SN,ST_SMS_IN) VALUES("+stID+",'"+name.toString()+"','"
                                    +sname.toString()+"','"+date.toString()+"','"+nick.toString()+"','"+email.toString()+"','0','ERR','ERR','ERR','0')");
                    if (ALL->exec())
                    {
                     summary = "{\"REG\":\"resultSelect\",\"name\":\"suc\"}";
                    }
            }
            socket->write(summary);
            socket->waitForBytesWritten(500);
            qDebug() << "______________________";
            qDebug() << summary;
            qDebug() << "______________________";
         }
        //Check_IN
        else if((doc.object().value("CHECK").toString() == "INFO"))
        {
            qDebug() << "______________________";
            qDebug() << "Data: "<<data;
            qDebug() << "______________________";
            QJsonValue id;
            QJsonValue fname;
            QJsonValue sname;

             QSqlQuery * logQ = new QSqlQuery(db);
              QSqlQuery * passQ = new QSqlQuery(db);
               QSqlQuery * idQ = new QSqlQuery(db);
                QSqlQuery * DataQ = new QSqlQuery(db);
                 QSqlQuery * NickQ = new QSqlQuery(db);
                  QSqlQuery * EmQ = new QSqlQuery(db);


            valuev = doc.object().value("check_data");
            array = valuev.toArray();

            foreach (const QJsonValue & v, array)
                        {
                                id = v.toObject().value("id").toString();
                                fname = v.toObject().value("fname").toString();
                                sname = v.toObject().value("sname").toString();
                        }

            if(db.open())
            {
                summary = "{\"CHECK\":\"resultSelect\",";
              if(logQ->exec("SELECT LOG_IN FROM LOG") && passQ->exec("SELECT PASS_IN FROM LOG") && idQ->exec("SELECT ID FROM LOG")
                      && DataQ->exec("SELECT Date FROM LOG") && NickQ->exec("SELECT Nickname FROM LOG") && EmQ->exec("SELECT Email FROM LOG"))
               {
                   while(idQ->next() && logQ->next() && passQ->next() && DataQ->next() && NickQ->next() && EmQ->next())
                   {
                        if((logQ->value(0).toString() == fname.toString() && passQ->value(0).toString() == sname.toString()))
                        {
                              summary.append("\"ID\":\"" + idQ->value(0).toString().toUtf8()

                                             + "\",\"fname\":\"" + logQ->value(0).toString().toUtf8()

                                             +"\",\"sname\":\"" + passQ->value(0).toString().toUtf8()

                                             +"\",\"Date\":\"" + DataQ->value(0).toString().toUtf8()

                                             +"\",\"Nick\":\"" + NickQ->value(0).toString().toUtf8()

                                             +"\",\"Email\":\"" + EmQ->value(0).toString().toUtf8() +"\"}");


                        }
                   }
               }
              if(summary == "{\"CHECK\":\"resultSelect\",") summary = "{\"CHECK\":\"resultSelect\",\"name\":\"name_error\"}";
                               socket->write(summary);
                               socket->waitForBytesWritten(1000);
                               qDebug() << "______________________";
                               qDebug()<<summary;
                               qDebug() << "______________________";
            }
            else
            {qDebug() << "______________________";
             qDebug() << "Query error";
             qDebug() << "______________________";
            }

        }
        //SMS
        else if((doc.object().value("SMS").toString() == "IN"))
        {
            QJsonValue sms;
            QJsonValue f_G_name;
            QJsonValue s_G_name;
            QJsonValue f_D_name;
            QJsonValue s_D_name;
            QSqlQuery * all = new QSqlQuery(db);

            valuev = doc.object().value("data");
            array = valuev.toArray();

            foreach (const QJsonValue & v, array)
                        {
                                sms = v.toObject().value("sms").toString();
                                f_G_name = v.toObject().value("f_gname").toString();
                                s_G_name = v.toObject().value("s_gname").toString();

                                f_D_name = v.toObject().value("f_dest_name").toString();
                                s_D_name = v.toObject().value("s_dest_name").toString();
                        }
            qDebug()<<"--------";
            qDebug()<<"Your fName: " + f_G_name.toString();
            qDebug()<<"--------";
            qDebug()<<"Your sName: " + s_G_name.toString();
            qDebug()<<"--------";
            qDebug()<<"Dest fName: " + f_D_name.toString();
            qDebug()<<"--------";
            qDebug()<<"Dest sName: " + s_D_name.toString();
            qDebug()<<"--------";

            summary = "{\"SMS\":\"resultSelect\",";
            if(db.open())
            {
             //sms_IN
             all->prepare("UPDATE LOG SET SMS_IN = '"+sms.toString()+"', SMS_IN_FROM_N = '"
                          +f_G_name.toString()+"',SMS_IN_FROM_SN = '"+s_G_name.toString()+"',ST_SMS_IN = '1' WHERE LOG_IN = '"+f_D_name.toString()+
                          "' and PASS_IN = '"+s_D_name.toString()+"'");
             if (all->exec())
             {
                 summary = "{\"SMS\":\"resultSelect\",\"status\":\"OK\"}";
                 socket->write(summary);
                 socket->waitForBytesWritten(1000);
                 qDebug()<<"--------";
                 qDebug()<<"TOTAL: " + summary;
             }
            }
            else
            {qDebug() << "______________________";
             qDebug() << "Query error";
             qDebug() << "______________________";
            }

        }
        //Select_Cust
        else if((doc.object().value("SELECT").toString() == "CUST"))
        {
            valuev = doc.object().value("data");
            array = valuev.toArray();

            foreach (const QJsonValue & v, array)
                        {
                                log = v.toObject().value("name").toString();
                                pass = v.toObject().value("sname").toString();
                        }
            if(db.open())
            {
                summary = "{\"SELECT\":\"resultSelect\",";
                QString log1;
                QString pass1;
               if(logQ->exec("SELECT LOG_IN FROM LOG") && passQ->exec("SELECT PASS_IN FROM LOG"))
               {
                   while(logQ->next()&&passQ->next())
                   {
                        if(logQ->value(0).toString() == log.toString() && passQ->value(0).toString() == pass.toString())
                        {
                             summary = "{\"SELECT\":\"resultSelect\",\"status\":\"find\"}";
                        }
                   }
               }
              if(summary == "{\"SELECT\":\"resultSelect\",") summary = "{\"SELECT\":\"resultSelect\",\"status\":\"find_error\"}";
                               socket->write(summary);
                               socket->waitForBytesWritten(1000);
                               qDebug() << "______________________";
                               qDebug() << summary;
                               qDebug() << "______________________";
            }
            else
            {qDebug() << "______________________";
             qDebug() << "Query error";
             qDebug() << "______________________";
            }


        }
        //Check changes
        else if((doc.object().value("CHECK").toString() == "SMS"))
        {
            valuev = doc.object().value("data");
            array = valuev.toArray();
            ///
            /// \brief ch_nameQ
            ///Ім'я того хто перевіряє
             QSqlQuery * ch_nameQ = new QSqlQuery(db);
            ///
            /// \brief ch_snameQ
            ///Прізвище того хто перевіряє
             QSqlQuery * ch_snameQ = new QSqlQuery(db);
             ///
             /// \brief statQ
             ///Статус повідомлення, 0 - немає, 1 - є
             QSqlQuery * statQ = new QSqlQuery(db);
             ///
             /// \brief smsQ
             ///SMS повідомлення
             QSqlQuery * smsQ = new QSqlQuery(db);
             ///
             /// \brief DnQ
             ///Ім'я того хто написав повідомлення
             QSqlQuery * DnQ = new QSqlQuery(db);
             ///
             /// \brief DsnQ
             ///Прізвище того хто написав повідомлення
             QSqlQuery * DsnQ = new QSqlQuery(db);
             QSqlQuery * updateQ = new QSqlQuery(db);
                 QJsonValue name;
                 QJsonValue sname;

                    summary = "{\"CHECK_SMS\":\"resultSelect\",";
            foreach (const QJsonValue & v, array)
                        {
                                name = v.toObject().value("name").toString();
                                sname = v.toObject().value("sname").toString();
                        }
            if(db.open())
                       {

                         if(ch_nameQ->exec("SELECT LOG_IN FROM LOG")
                                 && ch_snameQ->exec("SELECT PASS_IN FROM LOG")
                                 && statQ->exec("SELECT ST_SMS_IN FROM LOG")
                                 && smsQ->exec("SELECT SMS_IN FROM LOG")
                                 && DnQ->exec("SELECT SMS_IN_FROM_N FROM LOG")
                                 && DsnQ->exec("SELECT SMS_IN_FROM_SN FROM LOG"))
                          {
                              while(ch_nameQ->next()
                                    && ch_snameQ->next()
                                    && statQ->next()
                                    && smsQ->next()
                                    && DnQ->next()
                                    && DsnQ->next())
                              {
                                   if(ch_nameQ->value(0).toString() == name.toString()
                                   && ch_snameQ->value(0).toString() == sname.toString()
                                   && statQ->value(0).toString() == "1")
                                   {
                                       summary.append("\"Dname\":\"" + DnQ->value(0).toString().toUtf8()

                                                      + "\",\"Dsname\":\"" + DsnQ->value(0).toString().toUtf8()

                                                      +"\",\"sms\":\"" + smsQ->value(0).toString().toUtf8() +"\"}");

                                       updateQ->prepare("UPDATE LOG SET SMS_IN = 'ERR', SMS_IN_FROM_N = 'ERR', "
                                                        "SMS_IN_FROM_SN = 'ERR' ,ST_SMS_IN = '0' WHERE LOG_IN = '"+name.toString()+
                                                        "' and PASS_IN = '"+sname.toString()+"'");
                                       if (updateQ->exec())
                                       {
                                           qDebug()<<"--------";
                                           qDebug()<<"TOTAL: {\"CHECK_SMS\":\"resultSelect\",\"UPDATE\":\"OK\"}";
                                           break;
                                       }
                                       else
                                       {
                                           qDebug()<<"--------";
                                           summary = "{\"CHECK_SMS\":\"resultSelect\",\"status\":\"not\"}";
                                       }
                                   }

                              }
                          }
                         if(summary == "{\"CHECK_SMS\":\"resultSelect\",") {summary = "{\"CHECK_SMS\":\"resultSelect\",\"status\":\"not\"}";}
                         socket->write(summary);
                         socket->waitForBytesWritten(1000);

                       }
                    else
                       {
                        qDebug() << "______________________";
                        qDebug() << "Query error";
                        qDebug() << "______________________";
                       }
        }
        //SELECT MAIN CUST
        else if((doc.object().value("CUST").toString() == "SEL"))
        {
               QSqlQuery * STATQ = new QSqlQuery(db);
               QSqlQuery * nameQ = new QSqlQuery(db);
               QSqlQuery * snameQ = new QSqlQuery(db);
               QSqlQuery * ALLQ = new QSqlQuery(db);
               summary.clear();
               summary = "{\"SELMAIN\":\"resultSelect\",";
                if(db.open())
                {
                    if(nameQ->exec("SELECT LOG_IN FROM LOG")
                    && snameQ->exec("SELECT PASS_IN FROM LOG")
                    && STATQ->exec("SELECT LOG_STATUS FROM LOG"))
                    {
                        while(nameQ->next()
                              && snameQ->next()
                              && STATQ->next())
                        {
                            if(STATQ->value(0).toString() == "1")
                            {
                               summary.append("\"Dnname\":\"" + nameQ->value(0).toString().toUtf8() +"\",\"Dsnname\":\"" + snameQ->value(0).toString().toUtf8() +  "\"}");


                                ALLQ->prepare("UPDATE LOG SET Log_status = '0'");
                              if (ALLQ->exec())
                              {
                                  qDebug()<<"--------";
                                  qDebug()<<"TOTAL: {\"UPDATE\":\"resultSelect\",\"UPDATE\":\"OK\"}";
                                  break;
                              }
                              else
                              {
                                  qDebug()<<"--------";
                                  qDebug()<<"TOTAL: {\"UPDATE\":\"resultSelect\",\"UPDATE\":\"not\"}";
                              }
                              break;
                            }

                        }
                    }
                    else
                       {
                        qDebug() << "______________________";
                        qDebug() << "Query error";
                        qDebug() << "______________________";
                       }

                    if(summary == "{\"SELMAIN\":\"resultSelect\",")
                    {
                        summary = "{\"SELMAIN\":\"resultSelect\",\"status\":\"not\"}";
                    }


                  }
                socket->write(summary);
                socket->waitForBytesWritten(500);
                qDebug() << "Summary = " + summary;

                }


    }
}
void c_server::sockDisc()
{
    qDebug()<<"Disconnect";
    socket->deleteLater();
}

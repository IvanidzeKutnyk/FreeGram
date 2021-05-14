#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <QMainWindow>
#include <QTcpSocket>
#include <QMessageBox>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QStandardItemModel>
#include <QJsonArray>
#include <add_info.h>
#include <QVector>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
          QTcpSocket* socket;
        QByteArray data;

        QJsonDocument doc;
        QJsonParseError docError;


signals:
        void MySignal(QString);
public slots:
    void sockReady();
    void sockDisc();

private slots:
    void on_About_Button_clicked();
    void on_EXIT_Button_clicked();

    void on_IN_b_clicked();

    void on_Reg_b_clicked();

    void on_Connect_B_clicked();

private:
    Ui::MainWindow *ui;

};
#endif // MAINWINDOW_H

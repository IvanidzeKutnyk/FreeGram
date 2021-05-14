#include "mainwindow.h"
#include <QApplication>
//#include "QtSql/QSqlDatabase"
//#include "QSqlQuery"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow start;
    start.setMinimumSize(800,480);
    start.setMaximumSize(800,400);
    start.setWindowTitle("Login");
    start.show();
    return a.exec();
}

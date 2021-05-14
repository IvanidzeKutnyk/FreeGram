#include "exit.h"
#include "ui_exit.h"

EXIT::EXIT(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EXIT)
{
    ui->setupUi(this);
}

EXIT::~EXIT()
{
    delete ui;
}

//Кнопка NO
void EXIT::on_NO_B_clicked()
{
    this->close();
}
//Кнопка YES
void EXIT::on_YES_B_clicked()
{
    this->close();
}

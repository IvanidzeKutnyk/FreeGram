#include "customer_info.h"
#include "ui_customer_info.h"

customer_info::customer_info(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::customer_info)
{
    ui->setupUi(this);
}

customer_info::~customer_info()
{
    delete ui;
}

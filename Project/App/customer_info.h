#ifndef CUSTOMER_INFO_H
#define CUSTOMER_INFO_H

#include <QMainWindow>

namespace Ui {
class customer_info;
}

class customer_info : public QMainWindow
{
    Q_OBJECT

public:
    explicit customer_info(QWidget *parent = nullptr);
    ~customer_info();

private:
    Ui::customer_info *ui;
};

#endif // CUSTOMER_INFO_H

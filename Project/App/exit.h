#ifndef EXIT_H
#define EXIT_H

#include <QDialog>

namespace Ui {
class EXIT;
}

class EXIT : public QDialog
{
    Q_OBJECT

public:
    explicit EXIT(QWidget *parent = nullptr);
    ~EXIT();

private slots:

    void on_NO_B_clicked();

    void on_YES_B_clicked();

private:
    Ui::EXIT *ui;
};

#endif // EXIT_H

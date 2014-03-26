#include "dialog.h"
#include "ui_dialog.h"
#include "mainwindow.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    connect(ui->pushButton, SIGNAL(clicked()), (MainWindow* ) this->parent(),SLOT(show()));

    //connect the new connection signal to the new connection slot in the main window
    connect(this, SIGNAL(new_connection(QString, qint16)), (MainWindow*) this->parent(), SLOT(new_connection(QString, qint16)));
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::on_pushButton_clicked()
{
    //signal new connection
    emit new_connection(ui->lineEdit->text(), ui->lineEdit_2->text().toInt());
    hide();
}

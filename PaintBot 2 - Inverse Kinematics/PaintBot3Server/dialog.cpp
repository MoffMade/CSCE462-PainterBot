#include "dialog.h"
#include "ui_dialog.h"
#include "mainwindow.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::on_pushButton_clicked()
{
    if(ui->checkBox->isChecked()) emit runServer(ui->lineEdit->text().toInt());
    else emit runClient(ui->lineEdit_2->text(), ui->lineEdit->text().toInt());
    hide();
}

void Dialog::on_checkBox_clicked()
{
    if(ui->checkBox->isChecked()) ui->lineEdit_2->setEnabled(false);
    else ui->lineEdit_2->setEnabled(true);
}

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    move(500,300);
    SettingsDialog = new Dialog(this);

    //set up QTcpSocket
}

MainWindow::~MainWindow()
{
    delete ui;
    //delete dialog
    //delete QTcpSocket
}

void MainWindow::on_pushButton_clicked()
{
    SettingsDialog->show();
}

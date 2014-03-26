#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    move(500,300);
    m_SettingsDialog = new Dialog(this);

    //set up QTcpSocket and connect error handler signal/slot
    //the new connection signal is connected by the dialog upon creation
    m_socket = new QTcpSocket(this);
    connect(m_socket,SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(socket_error()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

//public
//called by dialog
void MainWindow::new_connection(QString host_address, qint16 host_port)
{
    m_socket->abort();
    m_socket->connectToHost(host_address,host_port);
    //change connection status label
}

//private
void MainWindow::on_pushButton_clicked()
{
    m_SettingsDialog->show();
}

void MainWindow::socket_error()
{
    //handle error
    qDebug("SOCKET ERROR: %s", m_socket->errorString().toStdString().c_str());
}

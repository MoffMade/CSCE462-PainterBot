#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_dialog = new Dialog(this);
    connect(m_dialog, SIGNAL(run(qint16, qint16)), this, SLOT(onRunSignal(qint16, qint16)));
    m_dialog->show();
    m_server = new QTcpServer(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::onRunSignal(qint16 port_number, qint16 delay)
{
    m_port_number = port_number;
    m_delay = delay;
    connect(m_server, SIGNAL(newConnection()), this, SLOT(acceptConnection()));
    if (!m_server->listen(QHostAddress::LocalHost, m_port_number)) qDebug("SERVER: %s", "Could not start Server");
    show();
    m_dialog->hide();
}

void MainWindow::acceptConnection()
{
    this->setWindowTitle("YEAH");
    m_socket = m_server->nextPendingConnection();
    connect(m_socket, SIGNAL(readyRead()), this, SLOT(readReady()));
    m_socket->write("yeah");
    m_socket->disconnectFromHost();
}

void MainWindow::readReady()
{
    //got reply from client
}

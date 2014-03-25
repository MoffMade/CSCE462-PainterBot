#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_dialog = new Dialog(this);
    connect(m_dialog, SIGNAL(run(qint16)), this, SLOT(onRunSignal(qint16)));
    m_dialog->show();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::onRunSignal(qint16 port_number)
{
    m_port_number = port_number;
    show();
    m_dialog->hide();
}

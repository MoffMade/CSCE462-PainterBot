#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    m_paint_flag = false;
    m_delay = 0;
    ui->setupUi(this);
    m_dialog = new Dialog(this);
    connect(m_dialog, SIGNAL(run(qint16)), this, SLOT(onRunSignal(qint16)));
    m_dialog->show();
    m_server = new QTcpServer(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::onRunSignal(qint16 port_number)
{
    m_port_number = port_number;
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

/***************************************************************/
//update functions
/***************************************************************/

void MainWindow::linkUpdate(double z, double theta1, double theta2)
{

}

void MainWindow::updateLink1(double z)
{

}

void MainWindow::updateLink2(double theta)
{

}


void MainWindow::updateLink3(double theta)
{

}

void MainWindow::worldUpdate(double x, double y)
{

}

void MainWindow::updateX(double x)
{

}

void MainWindow::updateY(double y)
{

}

void MainWindow::updateDelay(int seconds)
{

}

/***************************************************************/
//GUI widget Slots
/***************************************************************/


//link 1
void MainWindow::on_pushButton_2_clicked()
{

}

void MainWindow::on_pushButton_3_clicked()
{

}

void MainWindow::on_horizontalSlider_sliderMoved(int position)
{

}

//link 2
void MainWindow::on_pushButton_6_clicked()
{

}

void MainWindow::on_pushButton_7_clicked()
{

}

void MainWindow::on_horizontalSlider_2_sliderMoved(int position)
{

}

//link 3
void MainWindow::on_pushButton_8_clicked()
{

}

void MainWindow::on_pushButton_9_clicked()
{

}

void MainWindow::on_horizontalSlider_3_sliderMoved(int position)
{

}

//X position
void MainWindow::on_pushButton_10_clicked()
{

}

void MainWindow::on_pushButton_11_clicked()
{

}

//Y position

void MainWindow::on_pushButton_12_clicked()
{

}
void MainWindow::on_pushButton_13_clicked()
{

}

//paint flag
void MainWindow::on_pushButton_clicked()
{
    if(ui->pushButton->isChecked()){
        m_paint_flag = true;
    }
    else{
        m_paint_flag = false;
    }
}

//set delay
void MainWindow::on_pushButton_4_clicked()
{
    m_delay = ui->lineEdit->text().toInt();
}

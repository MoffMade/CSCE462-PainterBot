#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    //initialize link values, xy positions
    m_link1Value = 0;
    m_link2Value = 0;
    m_link3Value = 0;
    m_XValue = 0;
    m_YValue = 0;
    m_paint_flag = false;
    m_delay = 0;
    m_fromRemote = false;
    ui->setupUi(this);
    m_dialog = new Dialog(this);
    connect(m_dialog, SIGNAL(runServer(qint16)), this, SLOT(onRunServer(qint16)));
    connect(m_dialog, SIGNAL(runClient(QString,qint16)), this, SLOT(onRunClient(QString,qint16)));
    m_dialog->show();
    m_server = new QTcpServer(this);
    m_socket = 0;

    //generate scene
    m_scene = new QGraphicsScene(this);
    //create drawing format for the robot arm
    //set up brush (controls fill color of line)
    QBrush link_brush;
    link_brush.setColor(Qt::black);         //not really necessary
    link_brush.setStyle(Qt::SolidPattern);  //also not really necessary

    //set up pen (line style)
    m_link_pen.setBrush(link_brush);
    m_link_pen.setCapStyle(Qt::RoundCap);     //ends of lines are round
    m_link_pen.setJoinStyle(Qt::RoundJoin);   //joined lines are joined with round edges
    m_link_pen.setWidthF(10);                 //line width

    //draw robot arm base
    m_scene->addLine(-10,0,310,0,m_link_pen);

    //draw links; add lines to the scene in their initial positions
    m_link1 = m_scene->addLine(0,0,0,150,m_link_pen);     //150 pixels long
    m_link_pen.setColor(Qt::blue);
    m_link2 = m_scene->addLine(0,150,0,250,m_link_pen);   //100 pixels long
    m_link_pen.setColor(Qt::red);
    m_link3 = m_scene->addLine(0,250,0,315, m_link_pen);  //75 pixels long
    m_link_pen.setColor(Qt::yellow);

    //add scene to the QGraphicsView
    //the following line is needed to tell the QGraphicsView where to center its view. otherwise, it is centered on the collective center of objects in the scene.
    //the -2 is added to keep scroll bars from appearing on the edges, which happens if you set the SceneRect to the same size of the QGraphicsView.
    ui->graphicsView->setSceneRect(-100,-150,ui->graphicsView->width()-2, ui->graphicsView->height()-2);
    //this line flips the y axis; this means that (0,0) is at the bottom left corner instead of top left.
    ui->graphicsView->scale(0.7,-0.7);
    ui->graphicsView->setScene(m_scene);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onRunServer(qint16 port_number)
{
    setWindowTitle("PaintBot - Server");
    m_isServer = true;
    m_port_number = port_number;
    connect(m_server, SIGNAL(newConnection()), this, SLOT(acceptConnection()));
    if (!m_server->listen(QHostAddress::LocalHost, m_port_number)) qDebug("SERVER: %s", "Could not start Server");
    show();
    m_dialog->hide();
}

void MainWindow::onRunClient(QString address, qint16 port_number)
{
    m_isServer = false;
    setWindowTitle("PaintBot - Client");
    ui->lineEdit->setEnabled(false);
    m_socket = new QTcpSocket(this);
    m_socket->connectToHost(address, port_number);
    connect(m_socket, SIGNAL(readyRead()), this, SLOT(readReady()));
    connect(m_socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(socketError()));
    show();
    m_dialog->hide();
}

void MainWindow::acceptConnection()
{
    m_socket = m_server->nextPendingConnection();
    connect(m_socket, SIGNAL(readyRead()), this, SLOT(readReady()));
    connect(m_socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(socketError()));
}

void MainWindow::readReady()
{
    //read socket
    while(m_socket->canReadLine()){
        char d[2000];
        m_socket->readLine(d, 2000);
        qDebug("Recieved String: %s", d);
        QString data = QString(d);

        //parse string
        QStringList list = data.split(" ");
        m_link1Value = list.at(0).toDouble();
        m_link2Value = list.at(1).toDouble();
        m_link3Value = list.at(2).toDouble();
        m_XValue = list.at(3).toDouble();
        m_YValue = list.at(4).toDouble();
        double linkOrWorld = list.at(5).toDouble();

        //call update function
        m_fromRemote = true;
        if(linkOrWorld == 0) linkUpdate();
        if(linkOrWorld == 1) worldUpdate();
    }
}

void MainWindow::socketError()
{
    //handle error
    qDebug("SOCKET ERROR: %s", m_socket->errorString().toStdString().c_str());
}

void MainWindow::setUIEnabled(bool val)
{
    ui->pushButton->setEnabled(val);
    ui->pushButton_2->setEnabled(val);
    ui->pushButton_3->setEnabled(val);
    ui->pushButton_6->setEnabled(val);
    ui->pushButton_7->setEnabled(val);
    ui->pushButton_8->setEnabled(val);
    ui->pushButton_9->setEnabled(val);
    ui->pushButton_10->setEnabled(val);
    ui->pushButton_11->setEnabled(val);
    ui->pushButton_12->setEnabled(val);
    ui->pushButton_13->setEnabled(val);

    ui->horizontalSlider->setEnabled(val);
    ui->horizontalSlider_2->setEnabled(val);
    ui->horizontalSlider_3->setEnabled(val);
}

/***************************************************************/
//update functions
/***************************************************************/

//update based on link control
void MainWindow::update(std::vector<std::vector<double> > joints, std::vector<double> link_values)
{
    //set X and Y values
    m_XValue = joints[3][0];
    m_YValue = joints[3][1];

    //set new link values
    m_link1Value = link_values[0];
    m_link2Value = link_values[1];
    m_link3Value = link_values[3];

    //set link positions
    m_link1->setLine(joints[0][0], joints[0][1], joints[1][0], joints[1][1]);
    m_link2->setLine(joints[1][0], joints[1][1], joints[2][0], joints[2][1]);
    m_link3->setLine(joints[2][0], joints[2][1], joints[3][0], joints[3][1]);

    //check paint flag
    if(m_paint_flag){
        m_scene->addEllipse(joints[3][0], joints[3][1], 10, 10, m_link_pen);
    }

    //set sliders
    ui->horizontalSlider->setValue(qRound(m_link1Value));
    ui->horizontalSlider_2->setValue(qRound(m_link2Value));
    ui->horizontalSlider_3->setValue(qRound(m_link3Value));

    //set labels
    ui->label_4->setText(QString::number(m_link1Value));
    ui->label_6->setText(QString::number(m_link2Value));
    ui->label_7->setText(QString::number(m_link3Value));

    if(!m_fromRemote) remoteUpdate(0);
    m_fromRemote = false;
}

void MainWindow::linkUpdate()
{
    //kinematics calls: replace with appropriate functions
    std::vector<std::vector<double> > joints = FAKEsolver(m_link1Value, m_link2Value, m_link3Value);
    std::vector<double> link_values = FAKEgetLinkValues();
    update(joints, link_values);
}

//update based on world control
void MainWindow::worldUpdate()
{
    //kinematics calls: replace with appropriate functions
    std::vector<std::vector<double> > joints = FAKEsolver(m_XValue, m_YValue, m_link3Value);
    std::vector<double> link_values = FAKEgetLinkValues();
    update(joints, link_values);

}

//remote update
void MainWindow::remoteUpdate(double linkOrWorld)
{
    if((!m_isServer) || (m_isServer && m_delay == 0)){
        if(m_socket != 0){
            QString data;
            data.append(QString::number(m_link1Value));
            data.append(" ");
            data.append(QString::number(m_link2Value));
            data.append(" ");
            data.append(QString::number(m_link3Value));
            data.append(" ");
            data.append(QString::number(m_XValue));
            data.append(" ");
            data.append(QString::number(m_YValue));
            data.append(" ");
            data.append(QString::number(linkOrWorld));
            data.append("\n");
            m_socket->write(data.toStdString().c_str());
            m_socket->flush();
            qDebug("Sending String: %s", data.toStdString().c_str());
        }
    }
}

/***************************************************************/
//GUI widget Slots
/***************************************************************/


//link 1
void MainWindow::on_pushButton_2_clicked()
{
    m_link1Value++;
    linkUpdate();
}

void MainWindow::on_pushButton_3_clicked()
{
    m_link1Value--;
    linkUpdate();
}

void MainWindow::on_horizontalSlider_sliderMoved(int position)
{
    m_link1Value = position;
    linkUpdate();
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
    if(m_delay == 0) setUIEnabled(true);
    else setUIEnabled(false);
}

std::vector<std::vector<double> > MainWindow::FAKEsolver(double l1, double l2, double l3)
{
    // Random number between low and high
    //return qrand() % ((high + 1) - low) + low;

    std::vector<std::vector<double> > joints;
    for(int i = 0; i < 4; i++){
        std::vector<double> tmp;
        for(int j = 0; j < 2; j++){
            tmp.push_back(qrand() % ((100 + 1) - 0) + 0);
        }
        joints.push_back(tmp);
    }
    return joints;
}

std::vector<double> MainWindow::FAKEgetLinkValues()
{
    std::vector<double> values;
    values.push_back(m_link1Value);
    values.push_back(m_link2Value);
    values.push_back(m_link3Value);
    return values;
}

#include "mainwindow.h"
#include "ui_mainwindow.h"
//#include kinematics, i assume

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //generate scene
    QGraphicsScene * scene = new QGraphicsScene(this);
    //create drawing format for the robot arm
    //set up brush (controls fill color of line)
    QBrush link_brush;
    link_brush.setColor(Qt::black);         //not really necessary
    link_brush.setStyle(Qt::SolidPattern);  //also not really necessary

    //set up pen (line style)
    QPen link_pen;
    link_pen.setBrush(link_brush);
    link_pen.setCapStyle(Qt::RoundCap);     //ends of lines are round
    link_pen.setJoinStyle(Qt::RoundJoin);   //joined lines are joined with round edges
    link_pen.setWidthF(10);                 //line width

    //draw links; add lines to the scene in their initial positions
    m_link1 = scene->addLine(0,0,0,150,link_pen);     //150 pixels long
    link_pen.setColor(Qt::blue);
    m_link2 = scene->addLine(0,150,0,250,link_pen);   //100 pixels long
    link_pen.setColor(Qt::red);
    m_link3 = scene->addLine(0,250,0,315, link_pen);  //75 pixels long

    //store their initial D-H parameters
    link1_position = 0;
    link2_angle = 0;
    link3_angle = 0;

    //add scene to the QGraphicsView
    //the following line is needed to tell the QGraphicsView where to center its view. otherwise, it is centered on the collective center of objects in the scene.
    //the -2 is added to keep scroll bars from appearing on the edges, which happens if you set the SceneRect to the same size of the QGraphicsView.
    ui->graphicsView->setSceneRect(0,0,ui->graphicsView->width()-2, ui->graphicsView->height()-2);
    //this line flips the y axis; this means that (0,0) is at the bottom left corner instead of top left.
    ui->graphicsView->scale(1,-1);
    ui->graphicsView->setScene(scene);

    //connect signals and slots
    connect(ui->pushButton_2,SIGNAL(clicked()),this,SLOT(on_pushButton_2_clicked()));
    connect(ui->pushButton_3,SIGNAL(clicked()),this,SLOT(on_pushButton_3_clicked()));
    connect(ui->pushButton_6,SIGNAL(clicked()),this,SLOT(on_pushButton_6_clicked()));
    connect(ui->pushButton_7,SIGNAL(clicked()),this,SLOT(on_pushButton_7_clicked()));
    connect(ui->pushButton_8,SIGNAL(clicked()),this,SLOT(on_pushButton_8_clicked()));
    connect(ui->pushButton_9,SIGNAL(clicked()),this,SLOT(on_pushButton_9_clicked()));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    //toggle draw flag
}

void MainWindow::paint(){
    //add tiny ellipse at the m_link3's (x2,y2);
}

//slots for first link
void MainWindow::on_pushButton_2_clicked()
{
    //increment counter for link 1
}

void MainWindow::on_pushButton_3_clicked()
{
    //decrement counter for link 1
}

//slots for second link
void MainWindow::on_pushButton_6_clicked()
{
    //increment counter for link 2
}

void MainWindow::on_pushButton_7_clicked()
{
    //decrement counter for link 2
}

//slots for third link
void MainWindow::on_pushButton_8_clicked()
{
    //increment counter for link 3
}

void MainWindow::on_pushButton_9_clicked()
{
    //increment counter for link 3
}

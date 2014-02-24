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

    //add scene to the QGraphicsView
    //the following line is needed to tell the QGraphicsView where to center its view. otherwise, it is centered on the collective center of objects in the scene.
    //the -2 is added to keep scroll bars from appearing on the edges, which happens if you set the SceneRect to the same size of the QGraphicsView.
    ui->graphicsView->setSceneRect(0,0,ui->graphicsView->width()-2, ui->graphicsView->height()-2);
    //this line flips the y axis; this means that (0,0) is at the bottom left corner instead of top left.
    ui->graphicsView->scale(1,-1);
    ui->graphicsView->setScene(scene);

    //connect signals and slots
    connect(ui->horizontalSlider,SIGNAL(sliderMoved(int)),this,SLOT(on_horizontalSlider_sliderMoved(int)));
    connect(ui->horizontalSlider_2,SIGNAL(sliderMoved(int)),this,SLOT(on_horizontalSlider_2_sliderMoved(int)));
    connect(ui->horizontalSlider_3,SIGNAL(sliderMoved(int)),this,SLOT(on_horizontalSlider_3_sliderMoved(int)));

}

MainWindow::~MainWindow()
{
    delete ui;
}

//slider for link 1
void MainWindow::on_horizontalSlider_sliderMoved(int position)
{
    //tell new value of link 1 location according to value
    //ask for location of link 1
    //set link 1 position
    //ask for location of link 2
    //set link 2 position
    //ask for location of link 3
    //set link 3 position
    //paint if flag is set
}

//slider for link 2
void MainWindow::on_horizontalSlider_2_sliderMoved(int position)
{
    //tell new value of link 2 location according to value
    //link 1 doesnt change
    //ask for location of link 2
    //set link 2 position
    //ask for location of link 3
    //set link 3 position
    //paint if flag is set
}

//slider for link 3
void MainWindow::on_horizontalSlider_3_sliderMoved(int position)
{
    //tell new value of link 3 location according to value
    //link 1 doesnt change
    //link 2 doesnt change
    //ask for location of link 3
    //set link 3 position
    //paint if flag is set
}

void MainWindow::on_pushButton_clicked()
{
    //toggle draw flag
}

void MainWindow::paint(){
    //add tiny ellipse at the m_link3's (x2,y2);
}

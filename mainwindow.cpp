#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    L1(), L2(), L3(),
    solver(L1, L2, L3)
{
    ui->setupUi(this);

    //initialize paint flag
    m_paint_flag = false;

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

    //draw links; add lines to the scene in their initial positions
    m_link1 = m_scene->addLine(0,0,0,150,m_link_pen);     //150 pixels long
    m_link_pen.setColor(Qt::blue);
    m_link2 = m_scene->addLine(0,150,0,250,m_link_pen);   //100 pixels long
    m_link_pen.setColor(Qt::red);
    m_link3 = m_scene->addLine(0,250,0,315, m_link_pen);  //75 pixels long
    m_link_pen.setColor(Qt::yellow);

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
    ui->graphicsView->setScene(m_scene);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateRobot()
{
    //vector<vector<double> > update_theta(int link, double new_Theta);
    //vector<vector<double> > update_baseSlider(double newPos);
    //calculate new positions
    std::vector<std::vector<double> > points;
    //points = solver.updateBaseSlider(link1_position);

    //set positions of lines
    if(!points.empty()){
        m_link1->setLine(points[0][0], points[0][1], points[1][0], points[1][1]);
        m_link2->setLine(points[1][0], points[1][1], points[2][0], points[2][1]);
        m_link3->setLine(points[2][0], points[2][1], points[3][0], points[3][1]);

        paint(points[3][0], points[3][1]);
    }
    else{
        qDebug("forward kinematics calculation failure");
    }
}

void MainWindow::paint(double x, double y){
    //add 10px circle at the m_link3's (x2,y2);
    if(m_paint_flag){
        m_scene->addEllipse(x,y,10,10,m_link_pen);
    }
}

void MainWindow::on_pushButton_clicked()
{
    if(ui->pushButton->isChecked()){
        m_paint_flag = true;
    }
    else{
        m_paint_flag = false;
    }
}

//slots for first link
void MainWindow::on_pushButton_2_clicked()
{
    link1_position++;
    ui->label_4->setText(QString::number(link1_position));
    updateRobot();
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

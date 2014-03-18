//warning: the stuff below is HIDEOUS

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <qmath.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    L0(0,0,0,0),            // d is variable (prismatic)
    L1(90,150,0,0),         // non-variable transform
    L2(0,100,0,0),          // theta is variable (revolute)
    L3(0,75,0,0),
    solver(L0, L1, L2, L3)
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

    //draw robot arm base
    m_scene->addLine(-10,0,310,0,m_link_pen);

    //draw links; add lines to the scene in their initial positions
    m_link1 = m_scene->addLine(0,0,0,150,m_link_pen);     //150 pixels long
    m_link_pen.setColor(Qt::blue);
    m_link2 = m_scene->addLine(0,150,0,250,m_link_pen);   //100 pixels long
    m_link_pen.setColor(Qt::red);
    m_link3 = m_scene->addLine(0,250,0,315, m_link_pen);  //75 pixels long
    m_link_pen.setColor(Qt::yellow);

    //set end effector position
    end_effector_position.setX(0);
    end_effector_position.setY(315);

    //set end effector coordinate labels
    ui->label_9->setText(QString::number(end_effector_position.x()));
    ui->label_11->setText(QString::number(end_effector_position.y()));

    //store their initial D-H parameters
    link1_position = 0;
    link2_angle = 0;
    link3_angle = 0;

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


//actually sets positions of drawn lines
void MainWindow::updateRobot(vector<vector<double> > points)
{
    //set positions of lines
    if(!points.empty()){
        m_link1->setLine(points[0][0], points[0][1], points[1][0], points[1][1]);
        m_link2->setLine(points[1][0], points[1][1], points[2][0], points[2][1]);
        m_link3->setLine(points[2][0], points[2][1], points[3][0], points[3][1]);

        paint(points[3][0], points[3][1]);

        //set end_effector_position
        end_effector_position.setX(points[3][0]);
        end_effector_position.setY(points[3][1]);

        //set end effector coordinate labels
        ui->label_9->setText(QString::number(end_effector_position.x()));
        ui->label_11->setText(QString::number(end_effector_position.y()));
    }
    else{
        qDebug("forward kinematics calculation failure");
    }
}

//checks if the paint flag is set and if so, draws a circle
void MainWindow::paint(double x, double y){
    //add 10px circle at the m_link3's (x2,y2);
    if(m_paint_flag){
        m_scene->addEllipse(x,y,10,10,m_link_pen);
    }
}

//toggles paint flag
void MainWindow::on_pushButton_clicked()
{
    if(ui->pushButton->isChecked()){
        m_paint_flag = true;
    }
    else{
        m_paint_flag = false;
    }
}

//Link 1 functions
//updates all links based on a change in link 1
void MainWindow::updateLink1(){
    ui->label_4->setText(QString::number(link1_position));
    std::vector<std::vector<double> > points;
    points = solver.updateBaseSlider(link1_position);
    updateRobot(points);
}

//increment button slot for link 1
void MainWindow::on_pushButton_2_clicked()
{
    if (link1_position + 1 <= 300){
        link1_position++;
        updateLink1();
    }
    else qDebug("link 1 out of range");
}

//decrement button slot for link 1
void MainWindow::on_pushButton_3_clicked()
{
    if (link1_position - 1 >= 0){
        link1_position--;
        updateLink1();
    }
    else qDebug("link 1 out of range");
}

//slider for link 1
void MainWindow::on_horizontalSlider_sliderMoved(int position)
{
    link1_position = position;
    updateLink1();
}


//Link 2 functions
//updates all links (really just 2 and 3 change) based on link 2
void MainWindow::updateLink2(){
    ui->label_6->setText(QString::number(link2_angle));
    std::vector<std::vector<double> > points;
    points = solver.updateTheta(link2_angle, 2);        //*magic* number: represents link number
    updateRobot(points);
}

//increment angle button slot for link 2
void MainWindow::on_pushButton_6_clicked()
{
    if (link2_angle + 1 <= 359){
        link2_angle++;
        updateLink2();
    }
    else qDebug("link 2 is out of range");
}

//decrement angle button slot for link 2
void MainWindow::on_pushButton_7_clicked()
{
    if(link2_angle - 1 >= 0){
        link2_angle--;
        updateLink2();
    }
    else qDebug("link 2 is out of range");
}

//slider for link 2
void MainWindow::on_horizontalSlider_2_sliderMoved(int position)
{
    link2_angle = position;
    updateLink2();
}

//Link 3 functions
// updates link 3
void MainWindow::updateLink3(){
    ui->label_7->setText(QString::number(link3_angle));
    std::vector<std::vector<double> > points;
    points = solver.updateTheta(link3_angle, 3);        //*magic* number: represents link number
    updateRobot(points);
}

//increment angle button slot for link 3
void MainWindow::on_pushButton_8_clicked()
{
    if(link3_angle + 1 <= 359){
        link3_angle++;
        updateLink3();
    }
    else qDebug("link 3 is out of range");
}

//decrement angle button slot for link 3
void MainWindow::on_pushButton_9_clicked()
{
    if (link3_angle - 1 >= 0){
        link3_angle--;
        updateLink3();
    }
    else qDebug("link 3 is out of range");
}

//slider for link 3
void MainWindow::on_horizontalSlider_3_sliderMoved(int position)
{
    link3_angle = position;
    updateLink3();
}


//World Control
void MainWindow::on_pushButton_10_clicked()
{
    vector<vector<double> > points;
    end_effector_position.setX(end_effector_position.x() + 1);
    points = solver.inverseSolver(end_effector_position.x(), end_effector_position.y(), link2_angle + link3_angle - 90);
    updateRobot(points);
    link1_position = points[0][0];
    link2_angle = qAtan2((points[2][0] - points[1][0]), (points[2][1] - points[1][1]));
    link3_angle = qAtan2((points[3][0] - points[2][0]), (points[3][1] - points[2][1])) - link2_angle;
    ui->label_4->setText(QString::number(link1_position));
    ui->label_6->setText(QString::number(180*link2_angle/3.1415926535));
    ui->label_7->setText(QString::number(180*link3_angle/3.1415926535));
    ui->horizontalSlider->setValue(link1_position);
    ui->horizontalSlider_2->setValue(link2_angle);
    ui->horizontalSlider_2->setValue(link3_angle);
}

void MainWindow::on_pushButton_11_clicked()
{
    vector<vector<double> > points;
    end_effector_position.setX(end_effector_position.x() - 1);
    points = solver.inverseSolver(end_effector_position.x(), end_effector_position.y(), link2_angle + link3_angle - 90);
    updateRobot(points);
    link1_position = points[0][0];
    link2_angle = qAtan2((points[2][0] - points[1][0]), (points[2][1] - points[1][1]));
    link3_angle = qAtan2((points[3][0] - points[2][0]), (points[3][1] - points[2][1])) - link2_angle;
    ui->label_4->setText(QString::number(link1_position));
    ui->label_6->setText(QString::number(180*link2_angle/3.1415926535));
    ui->label_7->setText(QString::number(180*link3_angle/3.1415926535));
    ui->horizontalSlider->setValue(link1_position);
    ui->horizontalSlider_2->setValue(link2_angle);
    ui->horizontalSlider_2->setValue(link3_angle);
}

void MainWindow::on_pushButton_12_clicked()
{
    vector<vector<double> > points;
    end_effector_position.setY(end_effector_position.y() + 1);
    points = solver.inverseSolver(end_effector_position.x(), end_effector_position.y(), link2_angle + link3_angle - 90);
    updateRobot(points);
    link1_position = points[0][0];
    link2_angle = qAtan2((points[2][0] - points[1][0]), (points[2][1] - points[1][1]));
    link3_angle = qAtan2((points[3][0] - points[2][0]), (points[3][1] - points[2][1])) - link2_angle;
    ui->label_4->setText(QString::number(link1_position));
    ui->label_6->setText(QString::number(180*link2_angle/3.1415926535));
    ui->label_7->setText(QString::number(180*link3_angle/3.1415926535));
    ui->horizontalSlider->setValue(link1_position);
    ui->horizontalSlider_2->setValue(link2_angle);
    ui->horizontalSlider_2->setValue(link3_angle);
}

void MainWindow::on_pushButton_13_clicked()
{
    vector<vector<double> > points;
    end_effector_position.setY(end_effector_position.y() - 1);
    points = solver.inverseSolver(end_effector_position.x(), end_effector_position.y(), link2_angle + link3_angle - 90);
    updateRobot(points);
    link1_position = points[0][0];
    link2_angle = qAtan2((points[2][0] - points[1][0]), (points[2][1] - points[1][1]));
    link3_angle = qAtan2((points[3][0] - points[2][0]), (points[3][1] - points[2][1])) - link2_angle;
    ui->label_4->setText(QString::number(link1_position));
    ui->label_6->setText(QString::number(180*link2_angle/3.1415926535));
    ui->label_7->setText(QString::number(180*link3_angle/3.1415926535));
    ui->horizontalSlider->setValue(link1_position);
    ui->horizontalSlider_2->setValue(link2_angle);
    ui->horizontalSlider_2->setValue(link3_angle);
}

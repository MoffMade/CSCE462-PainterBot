#include "mainwindow.h"
#include "ui_mainwindow.h"
//#include kinematics, i assume

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //generate robot links in their initial positions

    //connect signals and slots
    connect(ui->horizontalSlider,SIGNAL(sliderMoved(int)),this,SLOT(on_horizontalSlider_valueChanged(int)));
    connect(ui->horizontalSlider_2,SIGNAL(sliderMoved(int)),this,SLOT(on_horizontalSlider_2_valueChanged(int)));
    connect(ui->horizontalSlider_3,SIGNAL(sliderMoved(int)),this,SLOT(on_horizontalSlider_3_valueChanged(int)));

}

MainWindow::~MainWindow()
{
    delete ui;
}

//slider for link 1
void MainWindow::on_horizontalSlider_valueChanged(int value)
{
    //tell new value of link 1 location according to value
    //ask for location of link 1
    //set link 1 position
    //ask for location of link 2
    //set link 2 position
    //ask for location of link 3
    //set link 3 position
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
}

//slider for link 3
void MainWindow::on_horizontalSlider_3_sliderMoved(int position)
{
    //tell new value of link 3 location according to value
    //link 1 doesnt change
    //link 2 doesnt change
    //ask for location of link 3
    //set link 3 position
}

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtGui>
#include "forwardKinematics.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private slots:

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_8_clicked();

    void on_pushButton_9_clicked();

    void on_horizontalSlider_sliderMoved(int position);

    void on_horizontalSlider_2_sliderMoved(int position);

    void on_horizontalSlider_3_sliderMoved(int position);

private:
    Ui::MainWindow *ui;

    QPen m_link_pen;
    QGraphicsScene *m_scene;
    QGraphicsLineItem *m_link1, *m_link2, *m_link3;
    double link1_position, link2_angle, link3_angle;
    DH_Param L0, L1, L2, L3;
    f_kin_solver solver;
    bool m_paint_flag;

    void updateRobot(vector<vector<double> > points);
    void updateLink1();
    void updateLink2();
    void updateLink3();
    void paint(double x, double y);
};

#endif // MAINWINDOW_H

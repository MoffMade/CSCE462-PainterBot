#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpServer>
#include <QTcpSocket>
#include <QGraphicsScene>



#include "dialog.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void onRunSignal(qint16 port_number);
    
private:
    Ui::MainWindow *ui;
    Dialog *m_dialog;

    QTcpServer *m_server;
    QTcpSocket *m_socket;
    qint16 m_port_number;

    QPen m_link_pen;
    QGraphicsScene *m_scene;
    QGraphicsLineItem *m_link1, *m_link2, *m_link3;

    int m_delay;
    bool m_paint_flag;

    double m_link1Value;
    double m_link2Value;
    double m_link3Value;

    double m_XValue;
    double m_YValue;

    //updates robot display based kinematics module
    void linkUpdate();
    void worldUpdate();
    void remoteUpdate(std::vector<std::vector<double> > joints);

private slots:
    void acceptConnection();
    void readReady();
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();
    void on_horizontalSlider_sliderMoved(int position);
    void on_pushButton_6_clicked();
    void on_pushButton_7_clicked();
    void on_horizontalSlider_2_sliderMoved(int position);
    void on_pushButton_8_clicked();
    void on_pushButton_9_clicked();
    void on_horizontalSlider_3_sliderMoved(int position);
    void on_pushButton_10_clicked();
    void on_pushButton_11_clicked();
    void on_pushButton_12_clicked();
    void on_pushButton_13_clicked();
    void on_pushButton_clicked();
    void on_pushButton_4_clicked();
};

#endif // MAINWINDOW_H

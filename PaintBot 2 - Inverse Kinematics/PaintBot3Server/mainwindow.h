#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpServer>
#include <QTcpSocket>
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
    int m_delay;
    bool m_paint_flag;

    //updates robot display based kinematics module
    void linkUpdate(vector<vector< double> > joints);

    //updates dh-parameter values
    void updateLink1(double z);
    void updateLink2(double theta);
    void updateLink3(double theta);

    //updates end-effector x and y locations
    void updateX(double x);
    void updateY(double y);

    //updates simulated delay
    void updateDelay(int seconds);

    //sends update data to client
    void remoteUpdate(vector<vector<double> > joints);

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

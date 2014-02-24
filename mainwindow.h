#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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
    void on_horizontalSlider_valueChanged(int value);

    void on_horizontalSlider_2_sliderMoved(int position);

    void on_horizontalSlider_3_sliderMoved(int position);

    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;

    QLineF * m_link1, m_link2, m_link3;

    void paint();
};

#endif // MAINWINDOW_H

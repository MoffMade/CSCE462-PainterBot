#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
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
    void new_connection(QString host_address, qint16 host_port);
    
private slots:
    void on_pushButton_clicked();
    void socket_error();
    void readReady();

private:
    Ui::MainWindow *ui;
    Dialog *m_SettingsDialog;

    QTcpSocket *m_socket;

    bool m_paint_flag;

    double m_link1Value;
    double m_link2Value;
    double m_link3Value;

    double m_XValue;
    double m_YValue;

    void linkUpdate();
    void worldUpdate();
    void remoteUpdate();

};

#endif // MAINWINDOW_H

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
};

#endif // MAINWINDOW_H

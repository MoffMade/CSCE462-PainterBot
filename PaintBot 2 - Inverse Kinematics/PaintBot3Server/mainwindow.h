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

private slots:
    void acceptConnection();
    void readReady();
};

#endif // MAINWINDOW_H

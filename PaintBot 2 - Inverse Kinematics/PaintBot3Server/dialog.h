#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();
    
signals:
    void runServer(qint16 port_number);
    void runClient(QString address, qint16 port_number);

private slots:
    void on_pushButton_clicked();

    void on_checkBox_clicked();

private:
    Ui::Dialog *ui;
};

#endif // DIALOG_H

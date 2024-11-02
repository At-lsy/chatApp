#ifndef REGISTERDLG_H
#define REGISTERDLG_H

#include <QWidget>
#include<QDialog>
#include<QPixmap>
#include"mytcpsocket.h"

namespace Ui {
class RegisterDlg;
}

class RegisterDlg : public QDialog
{
    Q_OBJECT

public:
    explicit RegisterDlg(QWidget *parent = nullptr);
    ~RegisterDlg();
    void set_headImage(QPixmap cut_pixmap);

private slots:
    void on_but_close_clicked();
    void on_but_head_clicked();
    void on_but_confirm_clicked();

signals:
    void creat_newAdmin(QString admin,QString password,QString username);
    void send_headHead(QPixmap pixmap);

private:
    Ui::RegisterDlg *ui;
    QString head_path="";
    MyTcpSocket* socket=nullptr;
    QThread* socket_thread=nullptr;
    QPixmap pic_pixmap;
};

#endif // REGISTERDLG_H

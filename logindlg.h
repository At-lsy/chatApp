#ifndef LOGINDLG_H
#define LOGINDLG_H

#include <QWidget>
#include<QThread>
#include<QString>
#include<QTimer>
#include<mydatabase.h>
#include<mytcpsocket.h>

QT_BEGIN_NAMESPACE
namespace Ui { class LoginDlg; }
QT_END_NAMESPACE

class LoginDlg : public QWidget
{
    Q_OBJECT

public:
    LoginDlg(QWidget *parent = nullptr);
    ~LoginDlg();
    void recv_loginResult(QString result);

signals:
    void send_login(QString admin,QString password);

private slots:
    void on_tbut_close_clicked();
    void on_tbut_mini_clicked();
    void on_but_login_clicked();
    void on_tbut_show_clicked();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_but_register_clicked();

private:
    Ui::LoginDlg *ui;
    QThread* tcp_thread=nullptr;
    MyTcpSocket* tcpSocket=nullptr;
    MyDataBase* myDB=nullptr;
    QTimer* timer=nullptr;
    QString myId;
};
#endif // LOGINDLG_H

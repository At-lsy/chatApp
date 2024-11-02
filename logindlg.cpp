#include "logindlg.h"
#include "ui_logindlg.h"
#include"winmove.h"

#include<QVector>
#include<maindlg.h>
#include"globals.h"
#include"registerdlg.h"


LoginDlg::LoginDlg(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::LoginDlg)
{
    ui->setupUi(this);
    //初始化
    this->setWindowFlag(Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground);
    this->installEventFilter(new WinMove);
    timer=new QTimer(this);
    QVector<QString> log={"登录中","登录中.","登录中..","登录中..."};
    connect(timer,&QTimer::timeout,this,[=](){
        //显示登录中效果
        static int i=0;
        ui->but_login->setText(log[i%4]);
        i++;
    });

    //套接字
    tcpSocket=new MyTcpSocket;
    tcp_thread=new QThread();
    tcpSocket->moveToThread(tcp_thread);
    tcp_thread->start();
    //发送账号密码
    connect(this,&LoginDlg::send_login,tcpSocket,&MyTcpSocket::send_Login);
    connect(tcpSocket,&MyTcpSocket::login_result,this,&LoginDlg::recv_loginResult);
    connect(tcpSocket,&MyTcpSocket::connectTimeOut,this,[=](){
        //登录连接超时
        ui->label_tip->setText("登陆超时！！！");
        timer->stop();
        ui->but_login->setText("登录");
        ui->but_login->setEnabled(true);
    });
}

LoginDlg::~LoginDlg()
{
    timer->deleteLater();
    delete ui;
}

void LoginDlg::recv_loginResult(QString result)
{
    if(result=="succeed"){
        //登录成功
        ui->label_tip->setText("登陆成功");
        MAIN_PATH=MAIN_PATH+"/"+this->myId;
        QMetaObject::invokeMethod(tcpSocket,"initDatabase",Qt::QueuedConnection);
        //初始化数据库
        MYID=this->myId;
        myDB=new MyDataBase(this->myId,this);
        if(!myDB->initDataBase("Login")){
            qDebug()<<"创建错误";
            return;
        }

        MainDlg* maindlg=new MainDlg(tcp_thread,tcpSocket,myId);
        maindlg->show();
        this->close();

    }
    if(result=="fail"){
        //登陆失败,密码错误
        ui->label_tip->setText("账号或密码错误！！！");
        timer->stop();
        ui->but_login->setText("登录");
        ui->but_login->setEnabled(true);
    }
}


void LoginDlg::on_tbut_close_clicked()
{
    if(tcpSocket!=nullptr){
        delete tcpSocket;
    }
    if(tcp_thread!=nullptr){
        tcp_thread->quit();
        tcp_thread->wait();
        tcp_thread->deleteLater();
    }
    if(timer!=nullptr){
        timer->stop();
        timer->deleteLater();
    }
    this->close();
}

void LoginDlg::on_tbut_mini_clicked()
{
    this->showMinimized();
}

void LoginDlg::on_but_login_clicked()
{
    QString admin=ui->lineEdit_admin->text();
    QString password=ui->lineEdit_password->text();
    if(admin==""||password==""){
        ui->label_tip->setText("请输入正确的账号密码！！！");
        return;
    }
    myId=admin;
    ui->label_tip->clear();
    ui->but_login->setDisabled(true);
    timer->start(500);
    emit send_login(admin,password);
}

void LoginDlg::on_tbut_show_clicked()
{
    static bool isShow=false;
    if(isShow){
        ui->lineEdit_password->setEchoMode(QLineEdit::Password);
        isShow=false;
    }
    else{
        ui->lineEdit_password->setEchoMode(QLineEdit::Normal);
        isShow=true;
    }
}

void LoginDlg::on_pushButton_clicked()
{
    myId="1066577840";
    emit send_login("1066577840","123456789");
}

void LoginDlg::on_pushButton_2_clicked()
{
    myId="1276926781";
    emit send_login("1276926781","123456789");
}

void LoginDlg::on_but_register_clicked()
{
    RegisterDlg r;
    r.exec();
}

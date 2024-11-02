#include "registerdlg.h"
#include "ui_registerdlg.h"

#include"winmove.h"
#include<QIcon>
#include<QPixmap>
#include<QDebug>
#include<QFileDialog>
#include<QPainter>
#include<QThread>
#include"selectheadimagedlg.h"

RegisterDlg::RegisterDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RegisterDlg)
{
    ui->setupUi(this);

    this->setWindowFlag(Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground);
    this->installEventFilter(new WinMove);

    socket=new MyTcpSocket;
    socket_thread=new QThread;

    socket->moveToThread(socket_thread);
    socket_thread->start();
    connect(socket,&MyTcpSocket::connectTimeOut,[=](){
        qDebug()<<"连接超时";
    });
    connect(this,&RegisterDlg::creat_newAdmin,socket,&MyTcpSocket::send_newAdmin);
    connect(this,&RegisterDlg::send_headHead,socket,&MyTcpSocket::send_headImage);
    connect(socket,&MyTcpSocket::request_headImage,[=](){
        emit send_headHead(pic_pixmap);
        ui->label_tip->setText("注册成功");
    });
}

RegisterDlg::~RegisterDlg()
{
    qDebug()<<"删除RegisterDlg";
    socket_thread->quit();
    socket_thread->wait();
    delete socket_thread;
    delete socket;
    delete ui;
}

void RegisterDlg::set_headImage(QPixmap cut_pixmap)
{
    pic_pixmap=cut_pixmap;

    QPixmap background_pix(ui->but_head->size());
    background_pix.fill(Qt::transparent);
    QPainter painter(&background_pix);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);
    QRegion region(background_pix.rect(),QRegion::Ellipse);
    painter.setClipRegion(region);
    painter.drawPixmap(background_pix.rect(),pic_pixmap.scaled(ui->but_head->size(),Qt::KeepAspectRatioByExpanding,Qt::SmoothTransformation));


    ui->but_head->setIcon(QIcon(background_pix));
    ui->but_head->setIconSize(ui->but_head->size());
}

void RegisterDlg::on_but_close_clicked()
{
    this->close();
}

void RegisterDlg::on_but_head_clicked()
{
    head_path=QFileDialog::getOpenFileName(this,"打开文件","C:/Users/MI/Desktop","ALL(*.*);;"
                                                                             "JPG,PNG(*.jpg,*png)");
    if(head_path=="") return;
//    QString style_head=R"(#but_head{
//                       background-color: rgb(0, 255, 0);
//                       border-radius:50px;
//                       border-image: url(:path);
//                   })";
//    style_head.replace(":path",head_path);
//    ui->but_head->setStyleSheet(style_head);
    SelectHeadImageDlg* select_dlg=new SelectHeadImageDlg(head_path);
    connect(select_dlg,&SelectHeadImageDlg::ensure_cut,[=](QPixmap cut_pixmap){
        set_headImage(cut_pixmap);
        select_dlg->close();
        delete select_dlg;
    });
    select_dlg->exec();
}

void RegisterDlg::on_but_confirm_clicked()
{
    QString username=ui->lineEdit_username->text();
    QString admin=ui->lineEdit_admin->text();
    QString password=ui->lineEdit_password->text();
    QString re_password=ui->lineEdit_repassword->text();
    if(pic_pixmap.isNull()){
        ui->label_tip->setText("还未选择头像");
        return;
    }
    else if(username==""||admin==""||password==""||re_password==""){
        ui->label_tip->setText("请输入完所有选项!");
        return;
    }
    else if(password!=re_password){
        ui->label_tip->setText("两次输入的密码不同!");
        return;
    }

    qDebug()<<"点击按钮:"<<admin<<" "<<password;
    emit creat_newAdmin(admin,password,username);
}

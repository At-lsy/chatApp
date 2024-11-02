#include "chatfileitemdlg.h"
#include "ui_chatfileitemdlg.h"

#include"globals.h"
#include<QFileInfo>
#include<QDir>
#include<QMouseEvent>
#include<QDesktopServices>
#include<QUrl>
#include<QDebug>

ChatFileItemDlg::ChatFileItemDlg(bool isLeft,QString filePath,QString clientId,QString myId,qint64 fileSize,QWidget *parent) :
    QWidget(parent),isLeft(isLeft),file_path(filePath),client_id(clientId),my_id(myId),file_Size(fileSize),
    ui(new Ui::ChatFileItemDlg)
{
    ui->setupUi(this);
    ui->file_name->setReadOnly(true);
    ui->progressBar->hide();
    ui->widget_3->installEventFilter(this);
    //初始化窗口
    initWidget();
}

ChatFileItemDlg::~ChatFileItemDlg()
{
    delete ui;
}

void ChatFileItemDlg::initWidget()
{
    //确认左右,设置头像
    if(isLeft){
        QString headPath=MAIN_PATH+"/head_image/"+client_id+".jpg";
        //qDebug()<<"设置左边头像"<<headPath;
        QPixmap pixmap(headPath);
        pixmap=pixmap.scaled(QSize(HEAD_WIDTH,HEAD_WIDTH),Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
        ui->left_head->setPixmap(pixmap);
        ui->widget_right->hide();
        ui->widget_left_2->hide();
    }
    else{
        QString headPath=MAIN_PATH+"/head_image/"+my_id+".jpg";
        //qDebug()<<"设置右边头像"<<headPath;
        QPixmap pixmap(headPath);
        pixmap=pixmap.scaled(QSize(HEAD_WIDTH,HEAD_WIDTH),Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
        ui->right_head->setPixmap(pixmap);
        ui->widget_left->hide();
        ui->widget_right_2->hide();
    }
    //设置文件名
    QStringList ls=file_path.split("/");
    QString fileName=ls[ls.size()-1];
    ui->file_name->setText(fileName);
    //设置文件大小
    float s=file_Size/1024.0;
    ui->file_size->setText(QString::number(s)+"kb");
    //设置文件提示
    ui->file_tip->setText("电脑文件");
}

void ChatFileItemDlg::sendFile()
{
    qDebug()<<"开始上传文件";
    if(fileSocket==nullptr){
        qDebug()<<"初始化发送文件套接字";
        fileSocket=new SendFileSocket(file_path,client_id,my_id);
        thread=new QThread;
        fileSocket->moveToThread(thread);
        thread->start();
    }
        //文件发送完成
    connect(fileSocket,&SendFileSocket::send_over,this,[=](){
        qDebug()<<"文件上传成功";
        ui->file_tip->setText("上传成功");
        ui->progressBar->hide();
        thread->quit();

        //线程关闭
        connect(thread,&QThread::finished,this,[=](){
            delete fileSocket;
            thread->deleteLater();
        },Qt::UniqueConnection);
        thread->wait();

    },Qt::UniqueConnection);
        //请求连接服务器结果
    connect(fileSocket,&SendFileSocket::connect_result,this,[=](bool isConnect){
            if(isConnect){//连接服务器成功，开始发送文件
                qDebug()<<"正在向服务器发送文件";
                ui->progressBar->setValue(0);
                ui->progressBar->show();
                //更新进度条
                connect(fileSocket,&SendFileSocket::current_send,ui->progressBar,&QProgressBar::setValue);
                ui->file_tip->setText("发送中...");
                qDebug()<<"开始调用发送文件";
                QMetaObject::invokeMethod(fileSocket,"send_File",Qt::QueuedConnection);
            }
            else{
                ui->progressBar->hide();
                ui->file_tip->setText("连接失败");
                this->isSend=false;
            }
    },Qt::UniqueConnection);

    qDebug()<<"开始调用初始化套接字";
    QMetaObject::invokeMethod(fileSocket,"initSocket",Qt::QueuedConnection);
}

void ChatFileItemDlg::clikedWin()
{
    if(!isSend){//如果文件发送失败
        sendFile();
    }

    if(isDownload){//在下载
        if(isPouse){
            qDebug()<<"下载继续";
            isPouse=false;
            return;
        }
        else{
            qDebug()<<"下载暂停";
            isPouse=true;
            return;
        }
    }
    else{
        QDir dir;
        if(dir.exists(file_path)){
            //打开文件
            qDebug()<<"该文件夹存在";
            QUrl fileUrl=QUrl::fromLocalFile(file_path);
            QDesktopServices::openUrl(fileUrl);
        }
        else{
            //请求下载文件
            QString path=MAIN_PATH+"/recv_file/"+file_path;
            QUrl fileUrl=QUrl::fromLocalFile(path);

            if(QDesktopServices::openUrl(fileUrl)) return;
            qDebug()<<"该文件不存在，请求下载";
            request_file();
        }

    }

}

void ChatFileItemDlg::request_file()
{
    if(fileSocket==nullptr){
        fileSocket=new SendFileSocket(file_path,client_id,my_id);
        thread=new QThread;
        fileSocket->moveToThread(thread);
        thread->start();
    }
        connect(fileSocket,&SendFileSocket::connect_result,this,[=](bool isConnect){
            if(isConnect){
                //连接成功，请求下载
                ui->progressBar->setValue(0);
                ui->progressBar->show();
                //更新进度条
                connect(fileSocket,&SendFileSocket::current_send,ui->progressBar,&QProgressBar::setValue,Qt::UniqueConnection);
                QMetaObject::invokeMethod(fileSocket,"request_file",Qt::QueuedConnection);
                isDownload=true;
                ui->file_tip->setText("下载中");
                //下载完成
                connect(fileSocket,&SendFileSocket::recv_over,this,[=](){
                    isDownload=false;
                    ui->progressBar->hide();
                    ui->file_tip->setText("下载完成");
                    thread->quit();
                    //线程任务完成
                    connect(thread,&QThread::finished,this,[=](){
                        delete fileSocket;
                        thread->deleteLater();
                    },Qt::UniqueConnection);

                    thread->wait();

                },Qt::UniqueConnection);
            }
            else{
                //连接失败
                ui->progressBar->hide();
                ui->file_tip->setText("连接失败，请检查网络连接");
            }
        },Qt::UniqueConnection);

    QMetaObject::invokeMethod(fileSocket,"initSocket",Qt::QueuedConnection);

}

bool ChatFileItemDlg::eventFilter(QObject *watched, QEvent *event)
{
    QWidget* w=static_cast<QWidget*>(watched);
    if(!w){
        return false;
    }
    if(w==ui->widget_3){
        if(event->type()==QMouseEvent::MouseButtonPress){
            static int i=0;
            qDebug()<<"鼠标点击了该页面:"<<i++;
            clikedWin();
        }
    }
}

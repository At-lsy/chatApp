#include "addfrienddlg.h"
#include "ui_addfrienddlg.h"

#include<QDebug>
#include"winmove.h"
#include"globals.h"

AddFriendDlg::AddFriendDlg(QString clientId,QString clientName,QWidget *parent) :
    QDialog(parent),client_id(clientId),client_name(clientName),
    ui(new Ui::AddFriendDlg)
{
    ui->setupUi(this);
    this->setWindowFlag(Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground);
    this->installEventFilter(new WinMove);

    ui->lineEdit_request->setText("我是");
    ui->lineEdit_remark->setText(client_name);
}

AddFriendDlg::~AddFriendDlg()
{
    qDebug()<<"删除AddFriendDlg";
    delete ui;
}

void AddFriendDlg::on_but_cancel_clicked()
{
    this->close();
    delete this;
}

void AddFriendDlg::on_but_ack_clicked()
{
    qDebug()<<"请求添加"<<client_id<<"为好友";
    QString msg=ui->lineEdit_request->text();
    QString remark=ui->lineEdit_remark->text();
    emit request_addFriend(status::WAIT_VERIFY,this->client_id,this->client_name,msg);
}

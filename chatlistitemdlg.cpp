#include "chatlistitemdlg.h"
#include "ui_chatlistitemdlg.h"

#include<QDebug>
#include"globals.h"

ChatListItemDlg::ChatListItemDlg(QString clientId, QString name, QDateTime time, QString message, int unreadNum,QWidget *parent) :
    QWidget(parent),client_Id(clientId),client_name(name),send_time(time),message(message),unreadNum(unreadNum),
    ui(new Ui::ChatListItemDlg)
{
    ui->setupUi(this);   
    init();
}

ChatListItemDlg::~ChatListItemDlg()
{
    delete ui;
}

void ChatListItemDlg::init()
{
    //还未设置头像
    QString head_path=MAIN_PATH+"/head_image"+"/"+client_Id+".jpg";
    QPixmap pixmap(head_path);
    pixmap=pixmap.scaled(QSize(50,50),Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
    ui->label_head->setPixmap(pixmap);

    //设置名称
    set_remark(this->client_name);

    if(message==""){
        ui->label_message->hide();
        ui->label_time->hide();
        ui->label_unreadnum->hide();
    }
    else{
        set_message(this->message);
        set_time(this->send_time);
        set_unreadNum(this->unreadNum);
    }

    //初始化右键菜单
    QAction* action_delete=new QAction("从聊天列表删除",this);
    QAction* action_other=new QAction("开发中",this);

    connect(action_delete,&QAction::triggered,[=](){
        qDebug()<<"删除该记录:"<<client_Id;
    });

    menu=new QMenu(this);
    menu->addAction(action_delete);
    menu->addAction(action_other);
}

QString ChatListItemDlg::get_clientName()
{
    return this->client_name;
}

QDateTime ChatListItemDlg::get_time()
{
    return this->send_time;
}

QString ChatListItemDlg::get_message()
{
    return this->message;
}

int ChatListItemDlg::get_unreadNum()
{
    return this->unreadNum;
}

void ChatListItemDlg::set_message(QString msg)
{
    this->message=msg;
    ui->label_message->setText(this->message);
    ui->label_message->show();
    set_time(QDateTime::currentDateTime());
    qDebug()<<"设置消息";
}

void ChatListItemDlg::set_time(QDateTime c_time)
{
    this->send_time=c_time;
    QString time=send_time.toString("HH:mm");
    ui->label_time->setText(time);
    ui->label_time->show();
    qDebug()<<"设置时间";
}

void ChatListItemDlg::set_remark(QString name)
{
    this->client_name=name;
    ui->label_name->setText(name);
}

void ChatListItemDlg::set_unreadNum(int num)
{
    this->unreadNum=num;
    if(this->unreadNum==0){
        ui->label_unreadnum->hide();
    }
    else{
        ui->label_unreadnum->setText(QString::number(num));
        ui->label_unreadnum->show();
    }
}

void ChatListItemDlg::add_unreadNum(QString msg)
{
    this->unreadNum=this->unreadNum+1;
    ui->label_unreadnum->setText(QString::number(this->unreadNum));
    qDebug()<<"设置未读";
    if(this->unreadNum>0){
        ui->label_unreadnum->show();
    }
    this->set_message(msg);
}

void ChatListItemDlg::transform_checked()
{
    emit hasCliked(this->client_Id,this->client_name);
}

void ChatListItemDlg::mousePressEvent(QMouseEvent *event)
{
    this->unreadNum=0;
    ui->label_unreadnum->hide();
    emit hasCliked(this->client_Id,this->client_name);
}

void ChatListItemDlg::contextMenuEvent(QContextMenuEvent *event)
{
    menu->popup(event->globalPos());
}

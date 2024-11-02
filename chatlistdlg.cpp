#include "chatlistdlg.h"
#include "ui_chatlistdlg.h"

#include<QListWidget>
#include<QListWidgetItem>
#include<QDir>
#include<QDebug>
#include<QString>
#include<QStringList>
#include<QTextStream>
#include<QSqlQuery>
#include"globals.h"


MyDataBase* ChatListDlg::chatlist_DB=nullptr;
ChatListDlg::ChatListDlg(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChatListDlg)
{
    ui->setupUi(this);
    qDebug()<<"创建ChatListDlg中";
    //初始化添加聊天记录聊天人
    if(chatlist_DB==nullptr){
        chatlist_DB=new MyDataBase(MYID);
        if(!chatlist_DB->initDataBase("ChatListDlg")){
            qDebug()<<"ChatListDlg初始化数据库失败";
        }
    }

    read_chatListItem();
    qDebug()<<"创建ChatListDlg成功";
}

ChatListDlg::~ChatListDlg()
{
    //保存聊天记录联系人
    qDebug()<<"保存chatlistDlg中";
    save_chatListItem();
    delete ui;
}

void ChatListDlg::add_chatListItem(QString clientId, QString name, QDateTime date, QString message, int unreadNum)
{
    ChatListItemDlg* listitem=new ChatListItemDlg(clientId,name,date,message,unreadNum);
    map_ChatListItem.insert(clientId,listitem);
    QListWidgetItem* item=new QListWidgetItem();
    item->setSizeHint(QSize(283,83));
    ui->listWidget->addItem(item);
    ui->listWidget->setItemWidget(item,listitem);
    connect(listitem,&ChatListItemDlg::hasCliked,this,&ChatListDlg::hasCliked);
    connect(listitem,&ChatListItemDlg::hasCliked,this,[=](){
        ui->listWidget->setCurrentItem(item);
    });
}

void ChatListDlg::read_chatListItem()
{
    qDebug()<<"读取read_chatListItem中";
    QString sql_string=R"(SELECT * from chatlist;)";
    QSqlQuery query=chatlist_DB->select_sql(sql_string);
    while(query.next()){
        QString id=query.value("friend_id").toString();
        QString remark=query.value("remark").toString();
        QString message=query.value("message").toString();
        int unread_count=query.value("unread_count").toInt();
        QDateTime time=query.value("send_time").toDateTime();
        this->add_chatListItem(id,remark,time,message,unread_count);
    }
    qDebug()<<"读取read_chatListItem成功";
}

void ChatListDlg::save_chatListItem()
{
    QMap<QString,ChatListItemDlg*>::iterator itr;
    for(itr=map_ChatListItem.begin();itr!=map_ChatListItem.end();itr++){
        //遍历聊天记录表
        QString id=itr.key();
        ChatListItemDlg* dlg=itr.value();
        QString name=dlg->get_clientName();
        QDateTime time=dlg->get_time();
        QString message=dlg->get_message();
        int unreadNum=dlg->get_unreadNum();
        chatlist_DB->save_chatList(id,name,time,message,unreadNum);
    }
}

void ChatListDlg::message_unRead(QString id, QString message)
{
    ChatListItemDlg* item=map_ChatListItem.value(id);
    item->add_unreadNum(message);
}

void ChatListDlg::text_change(QString clientId, QString text)
{
    ChatListItemDlg* itemDlg=map_ChatListItem.value(clientId);
    itemDlg->set_message(text);
}

bool ChatListDlg::client_exist(QString id)
{
    if(map_ChatListItem.contains(id)){
        return true;
    }
    return false;
}

ChatListItemDlg *ChatListDlg::get_itemDlg(QString id)
{
    return map_ChatListItem.value(id);
}

#include "newfrienddlg.h"
#include "ui_newfrienddlg.h"

#include<QListWidgetItem>
#include<QDebug>
#include<QSqlQuery>
#include"globals.h"


MyDataBase* NewFriendDlg::newFriendDlg_DB=nullptr;
NewFriendDlg::NewFriendDlg(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NewFriendDlg)
{
    ui->setupUi(this);
    init();
}

NewFriendDlg::~NewFriendDlg()
{
    delete ui;
}

void NewFriendDlg::addNewItem(quint16 state, QString id, QString name, QString msg)
{
//    if(map_itemDlg.contains(id)){
//        qDebug()<<"该item已存在";
//        NewFrientItemDlg* friendItem=map_itemDlg.value(id);
//        friendItem->change_butCondition(state);
//        if(state==status::HAS_ADD){
//            emit recv_agreeFriend(id,name);
//        }
//        return;
//    }
    QListWidgetItem* item=new QListWidgetItem;
    NewFrientItemDlg* n=new NewFrientItemDlg(state,id,name,msg);
    item->setSizeHint(QSize(0,100));
    ui->listWidget->addItem(item);
    ui->listWidget->setItemWidget(item,n);
    n->resize(100,ui->listWidget->rect().width());
    map_itemDlg.insert(id,n);

    //同意好友请求
    connect(n,&NewFrientItemDlg::agree_addFriend,this,&NewFriendDlg::agree_addFriend);
}

void NewFriendDlg::init()
{
    if(newFriendDlg_DB==nullptr){
        newFriendDlg_DB=new MyDataBase(MYID);
        if(!newFriendDlg_DB->initDataBase("NewFriendDlg")){
            qDebug()<<"NewFriendDlg数据库初始化失败";
        }
    }
    read_item();
}

void NewFriendDlg::updateItem()
{
    ui->listWidget->clear();
    this->map_itemDlg.clear();
    read_item();
}

void NewFriendDlg::read_item()
{
    QString sql_string=R"(select * from newfriend;)";
    QSqlQuery query=newFriendDlg_DB->select_sql(sql_string);
    while(query.next()){
        QString client_id=query.value("client_id").toString();
        QString username=query.value("username").toString();
        QString msg=query.value("message").toString();
        quint16 state=query.value("state").toUInt();
        addNewItem(state,client_id,username,msg);
    }
}


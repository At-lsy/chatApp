#include "searchfriendlistdlg.h"
#include "ui_searchfriendlistdlg.h"

#include<QDebug>
#include<QListWidgetItem>

SearchFriendListDlg::SearchFriendListDlg(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SearchFriendListDlg)
{
    ui->setupUi(this);
    //初始化添加好友item
    fItem=new SearchFriendListItemDlg;
    listItem=new QListWidgetItem;
    listItem->setSizeHint(QSize(285,75));
    ui->listWidget->addItem(listItem);
    ui->listWidget->setItemWidget(listItem,fItem);
    connect(fItem,&SearchFriendListItemDlg::search_friend,this,&SearchFriendListDlg::addFriend);
    listItem->setHidden(true);

}

SearchFriendListDlg::~SearchFriendListDlg()
{
    delete ui;
    delete fItem;
}

void SearchFriendListDlg::search_friend(QString id)
{
    qDebug()<<"搜索："<<id;
    if(!fItem->isVisible()){
        listItem->setHidden(false);
    }
    if(id==""){
        listItem->setHidden(true);
    }
    fItem->set_labelString(id);
}

QWidget *SearchFriendListDlg::get_widget()
{
    return ui->listWidget;
}


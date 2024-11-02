#include "searchfriendlistitemdlg.h"
#include "ui_searchfriendlistitemdlg.h"

#include<QDebug>

SearchFriendListItemDlg::SearchFriendListItemDlg(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SearchFriendListItemDlg)
{
    ui->setupUi(this);
}

SearchFriendListItemDlg::~SearchFriendListItemDlg()
{
    delete ui;
}

void SearchFriendListItemDlg::set_labelString(QString msg)
{
    QString s="查找用户:"+msg;
    this->id=msg;
    ui->label_string->setText(s);
}

void SearchFriendListItemDlg::mousePressEvent(QMouseEvent *event)
{
    //qDebug()<<"鼠标点击"<<this->id;
    //qDebug()<<"position:"<<this->mapToGlobal(this->rect().topRight());
    QPoint point=this->mapToGlobal(this->rect().topRight());
    emit search_friend(this->id,point);
}

#include "searchfriendsucceeddlg.h"
#include "ui_searchfriendsucceeddlg.h"

#include<QDebug>
#include<QPixmap>
#include"globals.h"

SearchFriendSucceedDlg::SearchFriendSucceedDlg(QString id,QString name,QWidget *parent) :
    QWidget(parent),my_id(id),my_name(name),
    ui(new Ui::SearchFriendSucceedDlg)
{
    ui->setupUi(this);
    this->setWindowFlag(Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground);
    this->setWindowFlag(Qt::Popup);
    ui->lineEdit_name->setReadOnly(true);

    //设置头像昵称
    QString image_path=MAIN_PATH+"/head_image/"+my_id+".jpg";
    qDebug()<<"image_path:"<<image_path;
    QPixmap pixmap(image_path);
    pixmap=pixmap.scaled(QSize(ui->label_head->width(),ui->label_head->height()));
    ui->label_head->setPixmap(pixmap);
    ui->lineEdit_name->setText(my_name);
}

SearchFriendSucceedDlg::~SearchFriendSucceedDlg()
{
    qDebug()<<"删除SearchFriendSucceedDlg";
    delete ui;
}

bool SearchFriendSucceedDlg::inFocus(QWidget* obj)
{
    if(obj==ui->widget||obj==ui->widget_3||obj==ui->but_add||obj==ui->lineEdit_name||obj==ui->label_head||qobject_cast<SearchFriendSucceedDlg*>(obj)){
        return true;
    }
    return false;
}

void SearchFriendSucceedDlg::changeFocus(QWidget *old, QWidget *now)
{
   qDebug()<<"焦点改变";
   qDebug()<<"当前焦点:"<<now;
   if(this->inFocus(now)){
       qDebug()<<"不用删除";
       return;
   }
   else{
       qDebug()<<"需要删除";
       ISSHOW_SEARCHFRIEND=false;
       delete this;
   }
}

void SearchFriendSucceedDlg::on_but_add_clicked()
{
    qDebug()<<"点击添加好友按钮";
    emit request_addFriend(this->my_id,this->my_name);
}

void SearchFriendSucceedDlg::focusOutEvent(QFocusEvent *event)
{
    if(this->geometry().contains(QCursor::pos())){
        qDebug()<<"不用删除";
        return;
    }
    else{
    qDebug()<<"需要删除";
        ISSHOW_SEARCHFRIEND=false;

        this->deleteLater();
    }
}

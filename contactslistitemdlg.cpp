#include "contactslistitemdlg.h".h"
#include "ui_contactslistitemdlg.h".h"

#include<QDebug>
#include"globals.h"

ContactsListItemDlg::ContactsListItemDlg(QString c_id,QString c_remark,QWidget *parent) :
    QWidget(parent),client_id(c_id),client_remark(c_remark),
    ui(new Ui::ContactsListItemDlg)
{
    ui->setupUi(this);
    init();
}

ContactsListItemDlg::~ContactsListItemDlg()
{
    delete ui;
}

void ContactsListItemDlg::init()
{
    if(this->client_id=="1"){
        //该item为新的朋友
        QString head_path=":/imageSource/newFriend_request.png";
        QPixmap pixmap(head_path);
        int size=ui->label_head->rect().width();
        pixmap=pixmap.scaled(QSize(size,size),Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
        ui->label_head->setPixmap(pixmap);

        ui->label_remark->setText(this->client_remark);
        return;
    }

    QString head_path=MAIN_PATH+"/head_image/"+this->client_id+".jpg";
    QPixmap pixmap(head_path);
    int size=ui->label_head->rect().width();
    pixmap=pixmap.scaled(QSize(size,size),Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
    ui->label_head->setPixmap(pixmap);

    ui->label_remark->setText(this->client_remark);
}

QString ContactsListItemDlg::get_remarkName()
{
    return this->client_remark;
}

void ContactsListItemDlg::mousePressEvent(QMouseEvent *event)
{
    qDebug()<<"点击页面";
    emit hasClicked(this->client_id,this->client_remark);
}

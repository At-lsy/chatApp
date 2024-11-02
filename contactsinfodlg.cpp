#include "contactsinfodlg.h"
#include "ui_contactsinfodlg.h"

#include"globals.h"
#include<QPixmap>

ContactsInfoDlg::ContactsInfoDlg(QString clientId,QString name,QWidget *parent) :
    QWidget(parent),client_id(clientId),remark(name),
    ui(new Ui::ContactsInfoDlg)
{
    ui->setupUi(this);
    init();
}

ContactsInfoDlg::~ContactsInfoDlg()
{
    delete ui;
}

void ContactsInfoDlg::init()
{
    //绘制头像
    QString path=MAIN_PATH+"/head_image/"+client_id+".jpg";
    int w=ui->label_head->width();
    QPixmap pixmap(path);
    pixmap=pixmap.scaled(QSize(w,w));
    ui->label_head->setPixmap(pixmap);
    //设置昵称
    ui->label_remark->setText(this->remark);
    //设置账号
    ui->label_clientId->setText(this->client_id);
}

QString ContactsInfoDlg::getId()
{
    return this->client_id;
}

QString ContactsInfoDlg::getName()
{
    return this->remark;
}

void ContactsInfoDlg::on_but_message_clicked()
{
    emit enterSendMessage(this->client_id,this->remark);
}

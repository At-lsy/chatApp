#include "newfrientitemdlg.h"
#include "ui_newfrientitemdlg.h"

#include<QDebug>
#include<QPixmap>
#include"globals.h"

NewFrientItemDlg::NewFrientItemDlg(quint16 state,QString id,QString name,QString msg,QWidget *parent) :
    QWidget(parent),item_status(state),client_id(id),client_name(name),client_msg(msg),
    ui(new Ui::NewFrientItemDlg)
{
    ui->setupUi(this);
    msg_status[0]="已添加";
    msg_status[1]="同意";
    msg_status[2]="等待验证";
    init();
}

NewFrientItemDlg::~NewFrientItemDlg()
{
    delete ui;
}

void NewFrientItemDlg::init()
{
    //设置头像
    QString image_path=MAIN_PATH+"/head_image/"+this->client_id+".jpg";
    QPixmap pixmap(image_path);
    pixmap=pixmap.scaled(ui->label_head->size(),Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
    ui->label_head->setPixmap(pixmap);

    ui->label_name->setText(client_name);
    ui->label_msg->setText(client_msg);
    change_butCondition(item_status);
}

void NewFrientItemDlg::setButtonStyle()
{
    ui->rbut_select->setStyleSheet("background:transparent; color:rgb(0,0,0);    border-radius:5px;");
}

void NewFrientItemDlg::change_butCondition(quint16 condition)
{
    qDebug()<<"按钮状态改变:"<<condition;
    switch(condition){
        case status::HAS_ADD:
            ui->rbut_select->setDisabled(true);
            ui->rbut_select->setText(msg_status[condition]);
            setButtonStyle();
            break;
        case status::WAIT_ARGEE:
            ui->rbut_select->setEnabled(true);
            ui->rbut_select->setText(msg_status[condition]);
            break;
        case status::WAIT_VERIFY:
            ui->rbut_select->setDisabled(true);
            ui->rbut_select->setText(msg_status[condition]);
            setButtonStyle();
            break;
        default:
            qDebug()<<"没有找到对应类型";
            break;
    }
}

void NewFrientItemDlg::on_rbut_select_clicked()
{
    item_status=status::HAS_ADD;
    emit agree_addFriend(this->client_id);
}

#include "searchfriendfaildlg.h"
#include "ui_searchfriendfaildlg.h"

#include<QDebug>
#include"winmove.h"

SearchFriendFailDlg::SearchFriendFailDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SearchFriendFailDlg)
{
    ui->setupUi(this);
    this->setWindowFlag(Qt::FramelessWindowHint);
    this->installEventFilter(new WinMove);
    ui->textEdit_tip->setReadOnly(true);
}

SearchFriendFailDlg::~SearchFriendFailDlg()
{
    qDebug()<<"删除SearchFriendFailDlg";
    delete ui;
}

void SearchFriendFailDlg::on_but_close_clicked()
{
    this->close();
    delete this;
}

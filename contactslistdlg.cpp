#include "contactslistdlg.h"
#include "ui_contactslistdlg.h"

#include"globals.h"
#include<QDebug>
#include<QFile>
#include<QTextStream>
#include<QSqlQuery>
#include<QListWidgetItem>

MyDataBase* ContactsListDlg::ContactList_DB=nullptr;

ContactsListDlg::ContactsListDlg(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ContactsListDlg)
{
    ui->setupUi(this);
    if(ContactList_DB==nullptr){
        ContactList_DB=new MyDataBase(MYID);
        if(!ContactList_DB->initDataBase("ContactsListDlg")){
            qDebug()<<"初始化失败";
        }
    }
    init();
}

ContactsListDlg::~ContactsListDlg()
{
    delete ui;
}

void ContactsListDlg::init()
{
    //取消搜索框的选中状态
    ui->listWidget->setFocus();
    //添加新朋友item
    add_contactItem("1","新的朋友");

    //读取联系人
    read_contacts();

    //隐藏关闭but
    ui->tbut_close->hide();
    connect(ui->tbut_close,&QToolButton::clicked,this,&ContactsListDlg::unchecked_lineEdit);
    //初始化搜索好友窗口
    fList=new SearchFriendListDlg;
    ui->vlayout->addWidget(fList);
    fList->hide();
    connect(ui->lineEdit_search,&QLineEdit::textChanged,fList,&SearchFriendListDlg::search_friend);
    //搜索框选中状态
    connect(qApp,&QApplication::focusChanged,[=](QWidget* oldWidget, QWidget* newWidget){
        if (qobject_cast<QLineEdit*>(newWidget)==ui->lineEdit_search||newWidget==fList->get_widget()) {
                checked_lineEdit();
                qDebug() << "QLineEdit 选中";
            }
    });
    //接收到添加好友消息
    connect(fList,&SearchFriendListDlg::addFriend,this,&ContactsListDlg::addFriend);
}

void ContactsListDlg::checked_lineEdit()
{
    ui->tbut_close->setVisible(true);
    ui->lineEdit_search->setFocus();

    //隐藏好友界面，显示搜索好友界面
    ui->listWidget->hide();
    fList->setVisible(true);
}

void ContactsListDlg::unchecked_lineEdit()
{
    ui->tbut_close->hide();

    //隐藏搜索好友界面，显示好友列表
    ui->lineEdit_search->clear();
    fList->hide();
    ui->listWidget->setVisible(true);
    ui->listWidget->setFocus();
}

void ContactsListDlg::add_contactItem(QString id, QString remark)
{
    if(map_itemDlg.contains(id)){
        qDebug()<<"该好友，已存在";
    }
    ContactsListItemDlg* friend_request=new ContactsListItemDlg(id,remark,this);
    QListWidgetItem* i=new QListWidgetItem;
    i->setSizeHint(QSize(283,75));
    ui->listWidget->addItem(i);
    ui->listWidget->setItemWidget(i,friend_request);
    connect(friend_request,&ContactsListItemDlg::hasClicked,[=](){
        ui->listWidget->setCurrentItem(i);
    });
    connect(friend_request,&ContactsListItemDlg::hasClicked,this,&ContactsListDlg::hasClicked);
    map_itemDlg.insert(id,friend_request);
}

void ContactsListDlg::read_contacts()
{
    QString sql_string=R"(SELECT friend_id,remark from friendship ORDER by remark;)";
    QSqlQuery query=ContactList_DB->select_sql(sql_string);
    while(query.next()){
        QString id=query.value("friend_id").toString();
        QString remark=query.value("remark").toString();
        add_contactItem(id,remark);
    }
}

void ContactsListDlg::update_contact()
{
    ui->listWidget->clear();
    delete_allItem();
    this->map_itemDlg.clear();
    add_contactItem("1","新的朋友");
    read_contacts();
}

void ContactsListDlg::delete_allItem()
{
    QMap<QString,ContactsListItemDlg*>::Iterator itor;
    for(itor=map_itemDlg.begin();itor!=map_itemDlg.end();itor++){
        ContactsListItemDlg* conItem=itor.value();
        delete conItem;
    }
}

bool ContactsListDlg::exist_client(QString id)
{
    return map_itemDlg.contains(id);
}

QString ContactsListDlg::get_remark(QString id)
{
    ContactsListItemDlg* contact=map_itemDlg.value(id);
    return contact->get_remarkName();
}


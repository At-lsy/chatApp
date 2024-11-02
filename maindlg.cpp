#include "maindlg.h"
#include "ui_maindlg.h"
#include"winmove.h"

#include<QDebug>
#include<QPixmap>
#include<QThread>
#include<QDir>
#include<QPoint>
#include<QEventLoop>
#include<QDialog>
#include<QSqlQuery>
#include"globals.h"
#include"chatlistitemdlg.h"
#include"searchfriendsucceeddlg.h"
#include"searchfriendfaildlg.h"
#include"addfrienddlg.h"

MyDataBase* MainDlg::mainDlg_DB=nullptr;
MainDlg::MainDlg(QThread* thread,MyTcpSocket* socket,QString myId,QWidget *parent) :
    QWidget(parent),socket_thread(thread),mysocket(socket),my_Id(myId),
    ui(new Ui::MainDlg)
{
    ui->setupUi(this);
    //改变全局默认文件路径
    if(mainDlg_DB==nullptr){
        mainDlg_DB=new MyDataBase(MYID);
        if(!mainDlg_DB->initDataBase("MainDlg")){
            qDebug()<<"初始化mainDlg_DB失败";
        }
    }

    this->setWindowFlag(Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground);
    this->installEventFilter(new WinMove);
    this->resize(1106,836);
    ui->rbut_message->setChecked(true);

    //设置中间页面
    chatList_layout=new QVBoxLayout(ui->widget_chatlist);
    chatList_layout->setContentsMargins(0,0,0,0);
    chatlistdlg=new ChatListDlg(this);
    chatList_layout->addWidget(chatlistdlg);
    //将当前的中间页面设为该页面
    mid_cur=chatlistdlg;

    //初始化联系人界面
    this->contactsListDlg=new ContactsListDlg;
    chatList_layout->addWidget(this->contactsListDlg);
    this->contactsListDlg->hide();
    connect(contactsListDlg,&ContactsListDlg::hasClicked,this,&MainDlg::showContactInfoDlg);
    //申请添加好友
    connect(contactsListDlg,&ContactsListDlg::addFriend,this,&MainDlg::request_searchFriend);
    //更新好友列表
    connect(mysocket,&MyTcpSocket::update_contact,contactsListDlg,&ContactsListDlg::update_contact);


    //设置右边页面
    chat_layout=new QVBoxLayout(ui->widget_chat);
    chat_layout->setContentsMargins(0,0,0,0);
    //点击联系人聊天，显示聊天界面
    connect(chatlistdlg,&ChatListDlg::hasCliked,this,&MainDlg::showChatDlg);

    //初始化好友添加列表界面
    newFrientListDlg=new NewFriendDlg;
    //同意好友申请,发送同意好友
    connect(newFrientListDlg,&NewFriendDlg::agree_addFriend,mysocket,&MyTcpSocket::send_agreeFriend);

    connect(newFrientListDlg,&NewFriendDlg::recv_agreeFriend,contactsListDlg,&ContactsListDlg::add_contactItem);
    connect(mysocket,&MyTcpSocket::update_newFriend,newFrientListDlg,&NewFriendDlg::updateItem);
    this->chat_layout->addWidget(newFrientListDlg);
    newFrientListDlg->hide();

    //设置头像
    QString head_path=MAIN_PATH+"/head_image/"+this->my_Id+".jpg";
    QPixmap pixmap(head_path);
    pixmap=pixmap.scaled(QSize(50,50),Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
    ui->label_head->setPixmap(pixmap);


    //套接字接收到文本消息
    connect(mysocket,&MyTcpSocket::give_text,this,&MainDlg::recv_text);
    //接收消息的窗口未打开
    connect(this,&MainDlg::message_unRead,chatlistdlg,&ChatListDlg::message_unRead);
    //套接字接收到文件消息
    connect(mysocket,&MyTcpSocket::give_fileMessage,this,&MainDlg::recv_fileMessage);
    //切换到联系人界面
    connect(ui->rbut_contacts,&QRadioButton::toggled,this,&MainDlg::checked_contacts);
    //切换到消息界面
    connect(ui->rbut_message,&QRadioButton::toggled,this,&MainDlg::checked_message);
    //显示搜索好友结果
    connect(mysocket,&MyTcpSocket::reFriend_result,this,&MainDlg::reFriend_result,Qt::UniqueConnection);
    //请求更新好友列表
    connect(this,&MainDlg::request_update_contact,mysocket,&MyTcpSocket::send_request_updateFriend);
    //请求更新新好友列表
    connect(this,&MainDlg::request_update_newFriend,mysocket,&MyTcpSocket::send_request_updateNewFriend);
}

MainDlg::~MainDlg()
{
    if(mysocket!=nullptr){
        mysocket->deleteLater();
    }
    if(socket_thread!=nullptr){
        socket_thread->quit();
        socket_thread->wait();
        socket_thread->deleteLater();
    }
    delete ui;
}

void MainDlg::checked_contacts()
{
    if(this->mid_cur!=this->contactsListDlg){
        this->mid_cur->hide();
        if(this->right_cur!=nullptr&&this->right_cur->isVisible()){
            this->right_cur->hide();
        }
        this->mid_cur=this->contactsListDlg;
        this->mid_cur->show();
    }
    else{
        qDebug()<<"已选择该页面";
    }
}

void MainDlg::checked_message()
{
    if(this->chatlistdlg==this->mid_cur){
        qDebug()<<"已选择该页面";
        return;
    }
    else{
        this->mid_cur->hide();
        if(this->right_cur!=nullptr&&this->right_cur->isVisible()){
            this->right_cur->hide();
        }
        this->mid_cur=this->chatlistdlg;
        this->mid_cur->show();
    }
}

//显示聊天界面
void MainDlg::showChatDlg(QString clientId,QString name)
{
    if(!map_chatDlg.contains(clientId)){
        qDebug()<<"未创建该聊天框";
        if(right_cur!=nullptr){
            right_cur->hide();
        }
        ChatDlg* chatdlg=new ChatDlg(clientId,name,my_Id);
        map_chatDlg.insert(clientId,chatdlg);
        chat_layout->addWidget(chatdlg);
        right_cur=chatdlg;
        //发送消息时改变聊天list的item
        connect(chatdlg,&ChatDlg::text_change,chatlistdlg,&ChatListDlg::text_change);
        //发送消息
        connect(chatdlg,&ChatDlg::send_Text,mysocket,&MyTcpSocket::send_Text);
        chatdlg->setBottom();
        return;
    }
    else{
        //已经创建过该对话框
        ChatDlg* chatdlg=map_chatDlg.value(clientId);
        if(right_cur!=chatdlg){//当前页面不为点击的页面
            qDebug()<<"不为点击页面";
            right_cur->hide();
            right_cur=chatdlg;
            chatdlg->show();
            chatdlg->setBottom();
        }
    }
}

void MainDlg::showContactInfoDlg(QString clientId, QString name)
{
    if(clientId=="1"){
        qDebug()<<"显示新朋友申请页面";
        showNewFriendDlg();
        return;
    }
    ContactsInfoDlg* contact=nullptr;
    if(!map_contactDlg.contains(clientId)){
        contact=new ContactsInfoDlg(clientId,name);
        connect(contact,&ContactsInfoDlg::enterSendMessage,this,&MainDlg::changeToMessage);
        map_contactDlg.insert(clientId,contact);
        this->chat_layout->addWidget(contact,0,Qt::AlignHCenter);
        contact->hide();
        qDebug()<<"该界面不存在";
    }
    else{
        contact=map_contactDlg.value(clientId);
    }

    if(right_cur!=nullptr){
        right_cur->hide();
        right_cur=contact;
        right_cur->show();
    }else{
        right_cur=contact;
        right_cur->show();
    }
}

void MainDlg::showNewFriendDlg()
{
    if(right_cur!=nullptr){
        right_cur->hide();
        right_cur=newFrientListDlg;
        right_cur->show();
    }else{
        right_cur=newFrientListDlg;
        right_cur->show();
    }
}

void MainDlg::show_addFriendDlg(QString id,QString name)
{
    qDebug()<<"显示添加好友界面";
    AddFriendDlg* a=new AddFriendDlg(id,name);
    //添加好友
    connect(a,&AddFriendDlg::request_addFriend,mysocket,&MyTcpSocket::send_addFriend);
    a->exec();
}

void MainDlg::recv_text(QString id, QString message)
{
    qDebug()<<"接收到消息"<<id<<":"<<message;
    int type=MessageType::TEXT_MESSAGE;
    save_message(type,id,message);
    if(!chatlistdlg->client_exist(id)){
        qDebug()<<"该item不存在";
        QString sql_string=R"(SELECT remark from friendship WHERE friend_id=:friend_id;)";
        sql_string.replace(":friend_id",id);
        QSqlQuery query = mainDlg_DB->select_sql(sql_string);
        query.next();
        QString remark=query.value("remark").toString();
        chatlistdlg->add_chatListItem(id,remark,QDateTime::currentDateTime(),"",0);
    }
    if(map_chatDlg.contains(id)){//该聊天窗口存在
        ChatDlg* dlg=map_chatDlg.value(id);
        QMetaObject::invokeMethod(dlg, "add_TextMessage", Qt::QueuedConnection,Q_ARG(QString,id),Q_ARG(QString,message),Q_ARG(bool,false));
        chatlistdlg->text_change(id,message);
        if(dlg->isVisible()){//该聊天窗口可见
            return;
        }
    }
    emit message_unRead(id,message);
}

void MainDlg::recv_fileMessage(QString sendId, QString fileName,quint64 fileSize)
{
    int type=MessageType::FILE_MESSAGE;
    save_fileMessage(type,sendId,fileSize,fileName);
    if(!chatlistdlg->client_exist(sendId)){
        qDebug()<<"该item不存在";
        return;
    }
    else{
        if(map_chatDlg.contains(sendId)){//该聊天窗口存在
            ChatDlg* dlg=map_chatDlg.value(sendId);
            QMetaObject::invokeMethod(dlg, "add_FileMessage", Qt::QueuedConnection,Q_ARG(QString,sendId),Q_ARG(QString,fileName),Q_ARG(quint64, fileSize),Q_ARG(bool,false));
            chatlistdlg->text_change(sendId,"文件消息");
            if(dlg->isVisible()){//该聊天窗口可见
                return;
            }
        }
    }
    emit message_unRead(sendId,"文件消息");
}


void MainDlg::save_message(int type, QString clientId, QString message)
{
    mainDlg_DB->save_message(type,clientId,clientId,message);
//    QString file_path=MAIN_PATH+"/contact/"+clientId+"/message_log.txt";
//    QString path=MAIN_PATH+"/contact/"+clientId;
//    QDir dir;
//    if(!dir.exists(path)){
//        //路径不存在
//        dir.mkdir(path);
//    }
//    QFile file(file_path);
//    file.open(QIODevice::Append|QIODevice::Text);
//    QTextStream data(&file);
//    data.setCodec("UTF-8");
//    data<<type<<" "<<clientId<<" "<<message<<"\n";
//    file.close();
//    qDebug()<<"保存接受的消息";
}

void MainDlg::save_fileMessage(int type, QString sendId, quint64 fileSize, QString fileName)
{
    QString msg=fileName+" "+QString::number(fileSize);
    mainDlg_DB->save_message(type,sendId,sendId,msg);
//    QString file_path=MAIN_PATH+"/contact/"+sendId+"/message_log.txt";
//    QString path=MAIN_PATH+"/contact/"+sendId;
//    QDir dir;
//    if(!dir.exists(path)){
//        //路径不存在
//        dir.mkdir(path);
//    }
//    QFile file(file_path);
//    file.open(QIODevice::Append|QIODevice::Text);
//    QTextStream data(&file);
//    data.setCodec("UTF-8");
//    data<<type<<" "<<sendId<<" "<<fileName<<" "<<fileSize<<"\n";
//    file.close();
//    qDebug()<<"保存接受的文件消息";
}

void MainDlg::changeToMessage(QString id,QString remark)
{
    qDebug()<<"changeToMessage";
    //切换到消息列表界面
    ui->rbut_message->setChecked(true);
    //检查该用户是否在聊天列表中
    if(this->chatlistdlg->client_exist(id)){
        //如果聊天列表存在该项则直接触发点击效果
        ChatListItemDlg* it=chatlistdlg->get_itemDlg(id);
        it->transform_checked();
    }
    else{
        //如果不存在，那么就先创建，再选中
        chatlistdlg->add_chatListItem(id,remark,QDateTime::currentDateTime(),"",0);
        ChatListItemDlg* it=chatlistdlg->get_itemDlg(id);
        it->transform_checked();
    }

}

//请求添加好友
void MainDlg::request_searchFriend(QString id, QPoint point)
{
    if(contactsListDlg->exist_client(id)){
        changeToMessage(id,contactsListDlg->get_remark(id));
        return;
    }

    qDebug()<<"申请添加："<<id<<"位置："<<point;
    this->reFriend_point=point;
    this->reFriend_id=id;
    quint16 type=MessageType::SEARCHFRIEND_MESSAGE;
    QMetaObject::invokeMethod(mysocket,"send_searchFriend",Qt::QueuedConnection,Q_ARG(QString,id));
    qDebug()<<"request_searchFriend:"<<QThread::currentThreadId();   
}

void MainDlg::reFriend_result(bool isFind, QString client_name)
{
    qDebug()<<"槽函数线程id:"<<QThread::currentThreadId()<<"id:"<<this->reFriend_id<<"point:"<<this->reFriend_point;
    if(isFind){
        //该用户存在
        if(!ISSHOW_SEARCHFRIEND){//此时界面没有显示的
            SearchFriendSucceedDlg* s=new SearchFriendSucceedDlg(this->reFriend_id,client_name);
            s->show();
            s->move(reFriend_point);
            s->setFocus();
            //connect(qApp,&QApplication::focusChanged,s,&SearchFriendSucceedDlg::changeFocus);
            //显示添加好友界面
            connect(s,&SearchFriendSucceedDlg::request_addFriend,this,&MainDlg::show_addFriendDlg);
            ISSHOW_SEARCHFRIEND=true;
        }

    }
    else{
        //该用户不存在
        QDialog* dialog=new SearchFriendFailDlg(this);

        QPoint point=QPoint(this->width()/2-165,this->height()/2-105)+this->mapToGlobal(QPoint(0,0));
        dialog->move(point);
        dialog->exec();
        //qDebug()<<"point:"<<point;
    }
}

void MainDlg::on_tbut_close_clicked()
{
    chatlistdlg->deleteLater();
    this->close();
}

void MainDlg::on_tbut_mini_clicked()
{
    this->showMinimized();
}

void MainDlg::on_tbut_normal_clicked()
{
    static bool isMax=false;
    if(!isMax){
        this->resize(1106,836);
        isMax=true;
    }
    else{
        this->showMaximized();
        isMax=false;
    }
}

void MainDlg::showEvent(QShowEvent *event)
{
    qDebug()<<"MainDlg显示成功";
    emit request_update_contact();
    emit request_update_newFriend();
}

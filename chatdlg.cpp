#include "chatdlg.h"
#include "ui_chatdlg.h"

#include"chattextitemdlg.h"
#include"chatfileitemdlg.h"
#include"globals.h"
#include<QListWidgetItem>
#include<QListWidget>
#include<QMouseEvent>
#include<QStringList>
#include<QTextStream>
#include<QScrollBar>
#include<QSqlQuery>
#include<QFile>
#include<QFileDialog>
#include<QFileInfo>
#include<QDir>
#include<QDebug>

MyDataBase* ChatDlg::chatDlg_DB=nullptr;
ChatDlg::ChatDlg(QString clientId,QString clientName,QString myId,QWidget *parent) :
    QWidget(parent),client_id(clientId),client_name(clientName),My_id(myId),
    ui(new Ui::ChatDlg)
{
    ui->setupUi(this);
    ui->label_name->setText(clientName);
    ui->textEdit_message->installEventFilter(this);

    //初始化database
    if(chatDlg_DB==nullptr){
       chatDlg_DB=new MyDataBase(this->My_id);
       if(!chatDlg_DB->initDataBase("ChatDlg")){
           qDebug()<<"创建数据库对象失败";
       }
       qDebug()<<"初始化数据库成功";
    }
    qDebug()<<"创建chatdlg成功";

    this->readMessage();
}

ChatDlg::~ChatDlg()
{
    delete chatDlg_DB;
    delete ui;
}

void ChatDlg::add_TextMessage(QString clientid,QString message,bool isRead)//最后一个参数表示是不是读取文件
{
    bool isLeft;
    if(clientid==My_id){
        isLeft=false;
    }
    else{
        isLeft=true;
    }
    chatTextItemDlg* text_dlg=new chatTextItemDlg(clientid,isLeft,message);
    QListWidgetItem* item=new QListWidgetItem();

    connect(text_dlg,&chatTextItemDlg::sendHight,[=](int height){
        item->setSizeHint(QSize(0,height));
    });
    if(isRead){
        ui->listWidget_chat->insertItem(0,item);
    }
    else{
        ui->listWidget_chat->addItem(item);
    }
    ui->listWidget_chat->setItemWidget(item,text_dlg);

}

void ChatDlg::add_FileMessage(QString sendId, QString filePath, quint64 fileSize, bool isRead)
{
    bool isLeft;
    if(sendId==My_id){
        isLeft=false;
    }
    else{
        isLeft=true;
    }
    ChatFileItemDlg* fileDlg=new ChatFileItemDlg(isLeft,filePath,client_id,My_id,fileSize,this);
    QListWidgetItem* item=new QListWidgetItem();
    item->setSizeHint(QSize(0,150));
    if(isRead){
        ui->listWidget_chat->insertItem(0,item);
    }
    else{
        ui->listWidget_chat->addItem(item);
    }
    ui->listWidget_chat->setItemWidget(item,fileDlg);

    if(!isLeft&&!isRead){
        qDebug()<<"选择的文件应该上传";
        fileDlg->sendFile();
        return;
    }
    qDebug()<<"选择的文件不应该上传";
}

void ChatDlg::add_chatItem(quint16 type, QString id, QString message)//读取添加
{
    qDebug()<<"增加聊天消息item"<<id<<":"<<message;
    switch(type){
        case MessageType::TEXT_MESSAGE:
            add_TextMessage(id,message,true);
            break;
        case MessageType::FILE_MESSAGE:
            QTextStream data(&message);
            QString fileName;
            quint64 fileSize;
            data>>fileName>>fileSize;
            add_FileMessage(id,fileName,fileSize,true);
    }
}

void ChatDlg::saveMessage(int type, QString saveId, QString message)//保存聊天记录
{
        chatDlg_DB->save_message(type,this->client_id,saveId,message);
//    QString file_path=MAIN_PATH+"/contact/"+this->client_id+"/message_log.txt";
//    QString path=MAIN_PATH+"/contact/"+this->client_id;
//    QDir dir;
//    if(!dir.exists(path)){
//        //路径不存在
//        dir.mkdir(path);
//    }
//    QFile file(file_path);
//    file.open(QIODevice::Append|QIODevice::Text);
//    QTextStream data(&file);
//    data.setCodec("UTF-8");
//    data<<type<<" "<<saveId<<" "<<message<<"\n";
//    file.close();
}

void ChatDlg::saveFileMessage(int type, QString saveId, QString fileName, quint64 fileSize)
{
    //存储到sqlite数据上
    QString msg=fileName+" "+QString::number(fileSize);
    chatDlg_DB->save_message(type,this->client_id,saveId,msg);
//    QString file_path=MAIN_PATH+"/contact/"+this->client_id+"/message_log.txt";
//    QString path=MAIN_PATH+"/contact/"+this->client_id;
//    QDir dir;
//    if(!dir.exists(path)){
//        //路径不存在
//        dir.mkdir(path);
//    }
//    QFile file(file_path);
//    file.open(QIODevice::Append|QIODevice::Text);
//    QTextStream data(&file);
//    data.setCodec("UTF-8");
//    data<<type<<" "<<saveId<<" "<<fileName<<" "<<fileSize<<"\n";
//    file.close();
}


void ChatDlg::readMessage()//读取聊天记录
{
    qDebug()<<"开始读取聊天记录";
    QString sql_string=R"(SELECT * from message WHERE msg_dlgid=:dlg_id ORDER by send_time DESC;)";
    sql_string.replace(":dlg_id",this->client_id);
    QSqlQuery query=chatDlg_DB->select_sql(sql_string);
    while(query.next()){
        quint16 type=query.value("message_type").toUInt();
        QString cliendId=query.value("send_id").toString();
        QString msg=query.value("content").toString();
        add_chatItem(type,cliendId,msg);
    }

    //用文件读取聊天记录
//    QString message_path=MAIN_PATH+"/contact/"+this->client_id+"/message_log.txt";
//    QFile file(message_path);
//    if(!file.open(QIODevice::ReadOnly|QIODevice::Text)){
//        qDebug()<<"打开聊天记录失败或者不存在";
//        return;
//    }

//    qint64 position=file.size();
//    int readLine=0;
//    QByteArray lineData;
//    while(position>0&&readLine<CHATLOG_NUM){
//        position--;
//        file.seek(position);
//        char byte;
//        file.read(&byte,1);
//        if(byte=='\n'){
//            if(!lineData.isEmpty()){
//                QString line=QString::fromUtf8(lineData);
//                QTextStream lineStream(&line);
//                int type;
//                QString id,message;
//                lineStream>>type>>id;
//                message=lineStream.readLine().trimmed();
//                add_chatItem(type,id,message);

//                readLine++;
//                lineData.clear();
//            }
//        }
//        else{//不是换行符，将数据添加到lineData
//            lineData.prepend(byte);
//        }
//    }

//    if(position==0&&readLine<CHATLOG_NUM){
//        QString line=QString::fromUtf8(lineData);
//        QTextStream lineStream(&line);
//        int type;
//        QString id,message;
//        lineStream>>type>>id;
//        message=lineStream.readLine().trimmed();
//        add_chatItem(type,id,message);
//    }
//    file.close();
}

void ChatDlg::setBottom()
{
    ui->listWidget_chat->verticalScrollBar()->setValue(ui->listWidget_chat->verticalScrollBar()->maximum());
}

bool ChatDlg::eventFilter(QObject *watched, QEvent *event)
{
    if(ui->textEdit_message==watched&&event->type()==QEvent::KeyPress){
        QKeyEvent* e=static_cast<QKeyEvent*>(event);
        if(e->key()==Qt::Key_Enter||e->key()==Qt::Key_Return){
            qDebug()<<"按下回车键！！";
            on_but_send_clicked();
            return true;
        }
    }
    return false;
}

void ChatDlg::on_but_send_clicked()
{
    QString message=ui->textEdit_message->toPlainText();
    if(message==""){
        qDebug()<<"消息为空";
        return;
    }
    add_TextMessage(this->My_id,message,false);
    qDebug()<<"my_id:"<<My_id<<"client_id"<<client_id;
    emit text_change(client_id,message);
    qint16 type=MessageType::TEXT_MESSAGE;
    emit send_Text(type,client_id,message);
    saveMessage(MessageType::TEXT_MESSAGE,this->My_id,message);
    ui->textEdit_message->clear();
    ui->listWidget_chat->scrollToBottom();
}

void ChatDlg::on_tbut_file_clicked()
{
    //发送文件
    QString file_path=QFileDialog::getOpenFileName(this,"打开文件","C:/Users/MI/Desktop","ALL(*.*);;"
                                                   "DOCX,DOC(*.docx,*doc);;"
                                                   "TXT(*.txt)");
    if(file_path=="") return;
    QStringList ls=file_path.split("/");
    QString filename=ls[ls.size()-1];
    QFileInfo fileInfo(file_path);
    quint64 fileSize=fileInfo.size();
    qDebug()<<"选择了文件路径为"<<file_path<<"的文件";
    this->add_FileMessage(this->My_id,file_path,fileSize,false);
    quint16 type=MessageType::FILE_MESSAGE;
    saveFileMessage(type,My_id,file_path,fileSize);
}

#include "mytcpsocket.h"

#include"globals.h"
#include<QDebug>
#include<QByteArray>
#include<QDir>
#include<QFile>
#include<QBuffer>
#include<QSqlQuery>
#include<QPixmap>

MyDataBase* MyTcpSocket::socket_DB=nullptr;
MyTcpSocket::MyTcpSocket(QObject *parent) : QObject(parent)
{

}

MyTcpSocket::~MyTcpSocket()
{
    if(myTcpSocket!=nullptr){
        myTcpSocket->close();
        myTcpSocket->deleteLater();
    }
}

bool MyTcpSocket::initSocket()
{
    qDebug()<<"初始化套接字";
    if(myTcpSocket==nullptr){
        myTcpSocket=new QTcpSocket;
    }
    myTcpSocket->connectToHost(SERVER_IP,SERVER_PORT.toUShort());
    if(!myTcpSocket->waitForConnected(30000)){
        qDebug()<<"连接超时";
        return false;
    }
    qDebug()<<"初始化套接字成功";
    connect(myTcpSocket,&QTcpSocket::readyRead,this,&MyTcpSocket::hasMessage);
    return true;
}

void MyTcpSocket::initVariable()
{
    this->has_readSize=0;
    this->all_msgSize=0;
    this->isReadSize=false;
    this->isReadType=false;
}

void MyTcpSocket::initDatabase()
{
    if(socket_DB==nullptr){
        socket_DB= new MyDataBase("/"+this->test_admin);
        if(!socket_DB->initDataBase("MyTcpSocket")){
            qDebug()<<"数据库MyTcpSocket初始化失败";
        }
        else{
            qDebug()<<"数据库MyTcpSocket初始化成功";
        }
    }
}

void MyTcpSocket::send_Login(QString admin, QString password)
{
    this->test_admin=admin;
    qDebug()<<"发送账号密码";
    if(myTcpSocket==nullptr||myTcpSocket->state()==QAbstractSocket::UnconnectedState){
        if(!initSocket()){
            emit connectTimeOut();
            return;
        }
    }
    QDataStream data(myTcpSocket);
    data.setByteOrder(QDataStream::BigEndian);
    quint16 msgType=MessageType::LOGIN_MESSAGE;
    quint64 msgSize=admin.toUtf8().size()+password.toUtf8().size();
    data<<msgType<<msgSize<<admin.toUtf8()<<password.toUtf8();
    myTcpSocket->flush();
}

void MyTcpSocket::send_newAdmin(QString admin, QString password,QString username)
{
    qDebug()<<"发送账号密码";
    if(myTcpSocket==nullptr||myTcpSocket->state()==QAbstractSocket::UnconnectedState){
        if(!initSocket()){
            emit connectTimeOut();
            return;
        }
    }
    QDataStream data(myTcpSocket);
    data.setByteOrder(QDataStream::BigEndian);
    quint16 msgType=MessageType::CREAT_NEWADMIN;
    quint64 msgSize=admin.toUtf8().size()+password.toUtf8().size()+username.toUtf8().size();
    data<<msgType<<msgSize<<admin.toUtf8()<<password.toUtf8()<<username.toUtf8();
}

void MyTcpSocket::send_Text(quint16 type, QString clientId, QString message)
{
    QDataStream stream(myTcpSocket);
    qint64 size=clientId.toUtf8().size()+message.toUtf8().size();
    stream<<type<<size<<clientId<<message;
    qDebug()<<"发送消息"<<message;
}

void MyTcpSocket::send_agreeFriend(QString client_id)
{
    quint8 state=status::HAS_ADD;
    quint16 type=MessageType::AGREEFRIEND_MESSAGE;
    quint64 size=client_id.toUtf8().size()+1;
    QDataStream data(myTcpSocket);
    data<<type<<size<<state<<client_id.toUtf8();
}

void MyTcpSocket::send_request_updateFriend()
{
    quint16 type=MessageType::UPDATE_REQUEST_CONTACT;
    quint64 size=8;
    quint64 last_update;
    QString sql_string = R"(SELECT max(last_update) FROM friendship;)" ;
    QSqlQuery query=socket_DB->select_sql(sql_string);
    if(query.next()){
        last_update=query.value(0).toULongLong();
    }
    else{
        last_update=0;
    }
    QDataStream data(myTcpSocket);
    data<<type<<size<<last_update;
    qDebug()<<"发送请求更新好友关系成功";
    return;
}

void MyTcpSocket::send_request_updateNewFriend()
{
    quint16 type=MessageType::UPDATE_NEWFRIEND;
    quint64 size=8;
    quint64 last_update;
    QString sql_string = R"(SELECT max(last_update) FROM newfriend;)" ;
    QSqlQuery query=socket_DB->select_sql(sql_string);
    if(query.next()){
        last_update=query.value(0).toULongLong();
    }
    else{
        last_update=0;
    }
    QDataStream data(myTcpSocket);
    data<<type<<size<<last_update;
    qDebug()<<"发送请求更新新好友列表成功";
    return;
}

void MyTcpSocket::send_request_begAddFriend()
{

}

void MyTcpSocket::send_headImage(QPixmap pixmap)
{
    qDebug()<<"发送头像中";
    QByteArray arry;
    QBuffer buffer(&arry);
    buffer.open(QIODevice::WriteOnly);
    pixmap.save(&buffer,"JPG");

    quint64 size=arry.size();
    qDebug()<<"头像大小："<<size;
    quint16 type=MessageType::HEADIMAGE_MESSAGE;
    QDataStream data(myTcpSocket);
    data<<type<<size<<arry;
}

void MyTcpSocket::send_addFriend(quint16 add_type,QString clientId,QString name,QString msg)
{
    quint16 type=MessageType::ADDFRIEND_MESSAGE;
    quint64 size=clientId.toUtf8().size()+name.toUtf8().size()+msg.toUtf8().size();
    QDataStream stream(myTcpSocket);
    stream<<type<<size<<add_type<<clientId.toUtf8()<<name.toUtf8()<<msg.toUtf8();
    qDebug()<<"发送好友添加请求成功";
}

void MyTcpSocket::send_searchFriend(QString id)
{
    QDataStream stream(myTcpSocket);
    quint16 type=MessageType::SEARCHFRIEND_MESSAGE;
    quint64 size=id.toUtf8().size();
    stream<<type<<size<<id.toUtf8();
    qDebug()<<"请求寻找好友:"<<id<<"消息大小："<<size;
}

void MyTcpSocket::hasMessage()
{
    qDebug()<<"有消息接收";
    if(myTcpSocket->bytesAvailable()==0){//如果没有消息可读
        return;
    }
    //接收消息头
    if(!isReadType){
        if(myTcpSocket->bytesAvailable()>2){
            QDataStream data(myTcpSocket);
            quint16 type;
            data>>type;
            this->msgType=type;
            isReadType=true;
            qDebug()<<"接收消息类型:"<<msgType;
        }
        else{
            return;
        }
    }
    //接收消息大小
    if(!isReadSize){
        if(myTcpSocket->bytesAvailable()>=8){
            quint64 msgSize;
            QDataStream data(myTcpSocket);
            data>>msgSize;
            this->all_msgSize=msgSize;
            isReadSize=true;
            qDebug()<<"接收消息大小:"<<all_msgSize;
        }
        else{
            return;
        }
    }

    switch(this->msgType){
        case MessageType::LOGIN_MESSAGE:
            qDebug()<<"接收到登录消息";
            recv_login();
            break;
        case MessageType::TEXT_MESSAGE:
            qDebug()<<"接收到文本消息";
            recv_text();
            break;
        case MessageType::FILE_MESSAGE:
            qDebug()<<"接收到了文件消息";
            recv_fileMessage();
            break;
        case MessageType::CONTACTS_MESSAGE:
            qDebug()<<"接收到用户信息";
            recv_contacts();
            break;
        case MessageType::SEARCHFRIEND_MESSAGE:
            qDebug()<<"接收到寻找好友信息";
            recv_searchFriend();
            break;
        case MessageType::HEADIMAGE_MESSAGE:
            qDebug()<<"接收到头像信息";
            recv_headImage();
            break;
        case MessageType::ADDFRIEND_MESSAGE:
            qDebug()<<"接收到好友添加信息";
            recv_addFriend();
            break;
        case MessageType::UPDATE_NEWFRIEND:
            qDebug()<<"接收到更新新好友消息";
            recv_updateNewFriend();
            break;
        case MessageType::UPDATE_CONTACT:
            qDebug()<<"接收到更新好友消息";
            recv_updateContact();
            break;
        case MessageType::GETIMAGE_NEWADMIN:
            qDebug()<<"接收到请求头像消息";
            emit request_headImage();
            initVariable();
            return;
    }
}

void MyTcpSocket::recv_login()
{
    //等待消息全部到达再接收
    if(myTcpSocket->bytesAvailable()<all_msgSize){
        return;
    }

    QDataStream data(myTcpSocket);
    QByteArray array;
    data>>array;
    QString result=QString::fromUtf8(array);
    emit login_result(result);


    initVariable();
    if(myTcpSocket->bytesAvailable()>0){
        hasMessage();
    }
}

void MyTcpSocket::recv_text()
{

    if(myTcpSocket->bytesAvailable()<all_msgSize){
        return;
    }

    QDataStream data(myTcpSocket);
    QByteArray id_b,message_b;
    data>>id_b>>message_b;
    QString id,message;
    id=QString::fromUtf8(id_b);
    message=QString::fromUtf8(message_b);
    qDebug()<<"接收到"<<id<<"的消息："<<message;
    emit give_text(id,message);
    initVariable();

    if(myTcpSocket->bytesAvailable()>0){
        hasMessage();
    }
}

void MyTcpSocket::recv_fileMessage()
{
    //data<<type<<size<<c_id<<fileName<<fileSize;

    if(myTcpSocket->bytesAvailable()<all_msgSize){
        return;
    }
    quint64 fileSize;
    fileSize=all_msgSize;
    QDataStream data(myTcpSocket);
    QString id,fileName;
    quint64 fSize;
    data>>id>>fileName>>fSize;
    emit give_fileMessage(id,fileName,fSize);
    initVariable();

    if(myTcpSocket->bytesAvailable()>0){
        hasMessage();
    }
}

void MyTcpSocket::recv_contacts()
{

    //data<<type<<size<<friend_id.toUtf8()<<remark.toUtf8()<<username.toUtf8()<<isdelete<<state<<last_update;
    if(myTcpSocket->bytesAvailable()<all_msgSize) return;

    QDataStream data(myTcpSocket);
    QByteArray id,name_r,name_u;
    quint8 isdelete,state;
    quint64 last_update;
    data>>id>>name_r>>name_u>>isdelete>>state>>last_update;
    QString friend_id,remark,username;
    friend_id=QString::fromUtf8(id);
    remark=QString::fromUtf8(name_r);
    username=QString::fromUtf8(name_u);

    QString sql_string=R"(SELECT * from friendship WHERE friend_id=:friend_id;)";
    sql_string.replace(":friend_id",friend_id);
    qDebug()<<"查询语句为"<<sql_string;
    QSqlQuery query=socket_DB->select_sql(sql_string);
    if(query.next()){
        qDebug()<<"存在该表项"<<"MYID:"<<MYID;
    }
    else{
        socket_DB->save_contact(friend_id,username,remark,state,isdelete,last_update);
    }

    initVariable();
    if(myTcpSocket->bytesAvailable()>0){
        hasMessage();
    }

//    //data<<type<<file_size;
//    qDebug()<<"接收用户信息中:"<<"可读消息大小:"<<myTcpSocket->bytesAvailable();
//    if(myTcpSocket->bytesAvailable()==0) return;

//    if(this->recv_File==nullptr){
//        qDebug()<<"文件对象未初始化";
//        QString path=MAIN_PATH+"/"+this->test_admin+"/contact";
//        QString file_path=MAIN_PATH+"/"+this->test_admin+"/contact/contact.txt";
//        QDir dir;

//        if(!dir.exists(path)){
//            dir.mkdir(path);
//        }

//        this->recv_File=new QFile(file_path);
//        if(!this->recv_File->open(QIODevice::WriteOnly)){
//            qDebug()<<"打开聊天人信息文件失败";
//            return;
//        }
//    }

//    if(has_readSize<all_msgSize){
//        QByteArray buffer=myTcpSocket->read(all_msgSize-has_readSize);
//        this->recv_File->write(buffer);
//        has_readSize+=buffer.size();
//        qDebug()<<"readSize:"<<has_readSize<<"fileSize:"<<all_msgSize;
//    }

//    if(has_readSize==all_msgSize){
//        qDebug()<<"文件接收完毕";
//        this->recv_File->close();
//        delete this->recv_File;
//        initVariable();
//    }

//    if(myTcpSocket->bytesAvailable()>0){
//        hasMessage();
//    }
}

void MyTcpSocket::recv_searchFriend()
{
    if(myTcpSocket->bytesAvailable()<all_msgSize){
        return;
    }

    //1、类型 2、消息大小 3、查询结果 4、用户昵称
    QDataStream data(myTcpSocket);
    QByteArray a_result,c_name;
    data>>a_result;
    QString result,client_name;
    if(a_result=="succeed"){
        data>>c_name;
        result=QString::fromUtf8(a_result);
        client_name=QString::fromUtf8(c_name);
        qDebug()<<"接收到查询结果："<<"结果："<<result<<"用户名:"<<client_name;
        emit reFriend_result(true,client_name);
    }
    else{
        qDebug()<<"接收到查询结果："<<"结果："<<result;
        emit reFriend_result(false,"");
    }

    //还原初始化
    initVariable();

    if(myTcpSocket->bytesAvailable()>0){
        hasMessage();
    }

}

void MyTcpSocket::recv_headImage()
{
    static QFile* image_file=nullptr;
    static bool isReadNameSize=false;//是否读取文件名称大小
    static bool isReadName=false;//是否读取文件名
    static quint32 imageName_size=0;//文件名大小
    QDataStream data(myTcpSocket);

    //1、文件类型 2、文件大小 3、文件名大小(4) 4、文件名 5、文件内容

    //如果还未读取文件名大小
    if(!isReadNameSize){
        if(myTcpSocket->bytesAvailable()<4) return;
        quint32 nSize;
        data>>nSize;
        imageName_size=nSize;
        qDebug()<<"文件名大小"<<imageName_size;
        isReadNameSize=true; 
    }

    //还未读取文件名
    if(!isReadName){
        if(myTcpSocket->bytesAvailable()<imageName_size) return;
        QByteArray iName;
        QString image_Name;
        data>>iName;
        image_Name=QString::fromUtf8(iName);
        qDebug()<<"文件头像名为："<<image_Name;

        //创建打开文件
        QString path=MAIN_PATH+"/head_image";
        QString image_path=MAIN_PATH+"/head_image/"+image_Name;
        QDir dir;
        if(!dir.exists(path)){
            dir.mkpath(path);
        }
        image_file=new QFile(image_path);
        image_file->open(QIODevice::WriteOnly);
        //改变标志位
        isReadName=true;
    }

    QByteArray arry=myTcpSocket->read(this->all_msgSize-this->has_readSize);
    this->has_readSize+=arry.size();
    image_file->write(arry);

    qDebug()<<"已读取大小："<<this->has_readSize<<"图像文件大小"<<this->all_msgSize;
    //读取完毕
    if(this->has_readSize==all_msgSize){
        qDebug()<<"读取完成";
        //文件指向空
        image_file->close();
        delete image_file;
        image_file=nullptr;

        //还原标志位
        isReadNameSize=false;
        isReadName=false;

        initVariable();
        qDebug()<<"初始化后已读取大小:"<<this->has_readSize<<"初始化后图像文件大小:"<<this->all_msgSize;
    }

    if(myTcpSocket->bytesAvailable()>0){
        qDebug()<<"缓冲区还有数据:"<<myTcpSocket->bytesAvailable();
        hasMessage();
    }
}

void MyTcpSocket::recv_addFriend()
{
    //QString friend_id, QString username, QString msg, quint8 state, quint64 last_update
    if(myTcpSocket->bytesAvailable()<all_msgSize) return;

    QByteArray id,name,msg;
    QString friend_id,username,message;
    quint8 state;
    quint64 last_update;

    QDataStream data(myTcpSocket);
    data>>id>>name>>msg>>state>>last_update;
    friend_id=QString::fromUtf8(id);
    username=QString::fromUtf8(name);
    message=QString::fromUtf8(msg);

    socket_DB->save_newFriend(friend_id,username,message,state,last_update);
    emit update_newFriend();

    initVariable();

    if(myTcpSocket->bytesAvailable()>0){
        hasMessage();
    }
}

void MyTcpSocket::recv_updateNewFriend()
{
    //data<<type<<size<<client_id.toUtf8()<<username.toUtf8()<<message.toUtf8()<<state<<last_update;
    if(myTcpSocket->bytesAvailable()<all_msgSize) return;

    QByteArray c_id,name,msg;
    quint8 state;
    quint64 last_update;
    QDataStream data(myTcpSocket);
    data>>c_id>>name>>msg>>state>>last_update;
    QString client_id=QString::fromUtf8(c_id);
    QString username=QString::fromUtf8(name);
    QString message=QString::fromUtf8(msg);
    qDebug()<<"接收到"<<client_id<<username<<message;

    QString select_string=R"(SELECT * from newfriend WHERE client_id=:client_id;)";
    select_string.replace(":client_id",client_id);
    QSqlQuery select_query=socket_DB->select_sql(select_string);

    if(select_query.next()){
        socket_DB->update_newFriend(client_id,username,message,state,last_update);
    }
    else{
        socket_DB->save_newFriend(client_id,username,message,state,last_update);
    }

    initVariable();
    emit update_newFriend();
    qDebug()<<"1111";

    if(myTcpSocket->bytesAvailable()>0){
        hasMessage();
    }
}

void MyTcpSocket::recv_updateContact()
{
    //friend_id.toUtf8()<<username.toUtf8()<<remark.toUtf8()<<state<<is_delete<<last_update;

    if(myTcpSocket->bytesAvailable()<all_msgSize) return;

    QByteArray f_id,name,mark;
    quint8 state,is_delete;
    quint64 last_update;
    QDataStream data(myTcpSocket);
    data>>f_id>>name>>mark>>state>>is_delete>>last_update;
    QString friend_id=QString::fromUtf8(f_id);
    QString username=QString::fromUtf8(name);
    QString remark=QString::fromUtf8(mark);

    qDebug()<<"接收到ID："<<friend_id;

    QString sql_string=R"(SELECT * from friendship WHERE friend_id=:friend_id;)";
    sql_string.replace(":friend_id",friend_id);
    QSqlQuery query= socket_DB->select_sql(sql_string);
    if(query.next()){
        socket_DB->update_contact(friend_id,username,remark,state,is_delete,last_update);
    }
    else{
        socket_DB->save_contact(friend_id,username,remark,state,is_delete,last_update);
    }

    emit update_contact();

    initVariable();

    if(myTcpSocket->bytesAvailable()>0){
        hasMessage();
    }
}

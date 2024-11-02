#include "sendfilesocket.h"
#include<QFile>
#include<QFileInfo>
#include<QDataStream>
#include<QByteArray>
#include<QDir>
#include<QDebug>

#include"globals.h"

SendFileSocket::SendFileSocket(QString filePath,QString clientId,QString myId,QObject *parent) : QObject(parent)
{
    this->file_path=filePath;//文件路径
    this->client_id=clientId;//对方id
    this->my_id=myId;//我的id
}

SendFileSocket::~SendFileSocket()
{
    qDebug()<<"sendFileSocket析构";
    if(fileSocket!=nullptr){
        fileSocket->disconnectFromHost();
        fileSocket->deleteLater();
    }
}

void SendFileSocket::recv_file(quint64 size)
{
    static QFile* file=nullptr;
    static quint64 readSize=0;
    static quint64 fileSize=size;
    static bool hasReady=false;
    if(!hasReady){
        QString file_dir=MAIN_PATH+"/recv_file";
        QDir dir;
        if(!dir.exists(file_dir)){
            dir.mkdir(file_dir);
        }
        QString path=file_dir+"/"+file_path;
        qDebug()<<"接收文件路径"<<path;
        file=new QFile(path);
        file->open(QIODevice::WriteOnly);
        hasReady=true;
        readFile=true;
    }

    QByteArray arry;
    arry=fileSocket->readAll();
    readSize+=arry.size();
    file->write(arry);
    int predcent=(readSize*100)/fileSize;
    emit current_send(predcent);

    if(readSize==fileSize){
        file->close();
        readFile=false;
        emit recv_over();
    }
}

void SendFileSocket::hasRecv()
{
    if(fileSocket->bytesAvailable()<10) return;
    if(readFile){
        recv_file(0);
        return;
    }
    QDataStream data(fileSocket);
    quint16 type;
    quint64 size;
    data>>type>>size;
    switch(type){
        case MessageType::TEXT_MESSAGE:
            break;
        case MessageType::FILE_SEND:
            recv_file(size);
            break;
    }
}

void SendFileSocket::initSocket()
{
    bool isConnect=true;
    if(this->fileSocket==nullptr){
        qDebug()<<"开始初始化qfileSocket";
        this->fileSocket=new QTcpSocket;
    }
    this->fileSocket->connectToHost(SERVER_IP,SERVER_PORT.toUShort());
    if(!this->fileSocket->waitForConnected(30000)){
        isConnect=false;
        qDebug()<<"连接服务器超时";
        emit connect_result(isConnect);
        return;
    }
    qDebug()<<"连接服务器成功";
    connect(fileSocket,&QTcpSocket::readyRead,this,&SendFileSocket::hasRecv,Qt::UniqueConnection);
    connect(fileSocket,&QTcpSocket::disconnected,this,&SendFileSocket::send_over);
    emit connect_result(isConnect);
}

void SendFileSocket::send_File()
{
    QFile file(this->file_path);
    if(!file.open(QIODevice::ReadOnly)){
        qDebug()<<"文件打开失败";
        return;
    }
    QStringList ls=file_path.split("/");
    QString fileName=ls[ls.size()-1];
    QFileInfo fileInfo(file_path);
    quint16 type=MessageType::FILE_SEND;//类型
    quint64 fileSize=fileInfo.size();//文件大小
    quint64 msgSize=fileName.toUtf8().size()+my_id.toUtf8().size()+client_id.toUtf8().size();
    qDebug()<<"该文件为"<<my_id<<"发送给"<<client_id;
    qDebug()<<"文件名是："<<fileName<<"文件大小是："<<fileSize;

    QDataStream data(this->fileSocket);
    //发送文件的头部(类型4，文件大小8，前缀大小8，文件名字4，发送给4，谁发送4)
    data<<type<<fileSize<<msgSize<<fileName.toUtf8()<<client_id.toUtf8()<<my_id.toUtf8();
    quint64 readSize=0;
    while(readSize<fileSize){
        QByteArray arry=file.readLine();
        readSize+=arry.size();
        int predcent=(readSize*100)/fileSize;
        emit current_send(predcent);
        fileSocket->write(arry);
        qDebug()<<"readSize:"<<readSize;
    }
    file.close();
    fileSocket->flush();
    qDebug()<<"缓冲区："<<fileSocket->bytesAvailable()<<"readSize:"<<readSize;
}

void SendFileSocket::request_file()
{
    QStringList ls=file_path.split("/");
    QString fileName=ls[ls.size()-1];
    quint16 type=MessageType::FILE_REQUEST;
    quint64 size=fileName.toUtf8().size()+this->my_id.toUtf8().size();
    QDataStream data(fileSocket);
    data<<type<<size<<this->my_id.toUtf8()<<fileName.toUtf8();
}

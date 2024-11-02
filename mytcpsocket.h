#ifndef MYTCPSOCKET_H
#define MYTCPSOCKET_H

#include <QObject>
#include<QFile>
#include<QTcpSocket>
#include<QDataStream>
#include<mydatabase.h>

class MyTcpSocket : public QObject
{
    Q_OBJECT
public:
    explicit MyTcpSocket(QObject *parent = nullptr);
    ~MyTcpSocket();
    bool initSocket();
    void initVariable();//初始化(还原)接收信息的变量
    void send_Login(QString admin,QString password);
    void send_newAdmin(QString admin,QString password,QString username);
    void send_Text(quint16 type,QString clientId,QString message);
    void send_agreeFriend(QString client_id);
    void send_request_updateFriend();
    void send_request_updateNewFriend();
    void send_request_begAddFriend();
    void send_headImage(QPixmap pixmap);
    void hasMessage();
    void recv_login();
    void recv_text();
    void recv_fileMessage();
    void recv_contacts();
    void recv_searchFriend();
    void recv_headImage();
    void recv_addFriend();
    void recv_updateNewFriend();
    void recv_updateContact();

public slots:
    void send_searchFriend(QString id);
    void send_addFriend(quint16 add_type,QString clientId,QString name,QString msg);
    void initDatabase();

signals:
    void connectTimeOut();
    void login_result(QString result);
    void give_text(QString id,QString message);
    void give_fileMessage(QString id,QString fileName,quint64 fileSize);
    void reFriend_result(bool isFind,QString client_name);
    void update_newFriend();
    void update_contact();
    void request_headImage();

private:
    QTcpSocket* myTcpSocket=nullptr;
    QFile* recv_File=nullptr;
    QString test_admin;
    quint16 msgType;
    quint64 has_readSize=0;//已读消息大小
    quint64 all_msgSize=0;//未读消息大小
    bool isReadType=false;
    bool isReadSize=false;

    static MyDataBase* socket_DB;
};

#endif // MYTCPSOCKET_H

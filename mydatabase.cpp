#include "mydatabase.h"

#include<QDebug>
#include<QSqlQuery>
#include<QDir>
#include<QSqlError>
#include"globals.h"

MyDataBase::MyDataBase(QString myId,QObject *parent) : QObject(parent),my_id(myId)
{
    this->db_path=MAIN_PATH+"/"+myId+".db";
}

MyDataBase::~MyDataBase()
{
    db.close();
}

bool MyDataBase::initDataBase(QString dbName)
{
    db=QSqlDatabase::addDatabase("QSQLITE",dbName);
    QDir dir;
    if(!dir.exists(this->db_path)){
        dir.mkpath(MAIN_PATH);
        return creat_database();
    }
    db.setDatabaseName(this->db_path);
    if(!db.open()){
        qDebug()<<"初始化失败";
        return false;
    }
}

bool MyDataBase::creat_database()
{
    db.setDatabaseName(this->db_path);
    if(!db.open()){
        qDebug()<<"打开数据库失败";
        return false;
    }
    QSqlQuery query(db);
    //创建好友表
    QString sql_friends=R"(CREATE TABLE IF NOT EXISTS friendship(
                    friend_id  INTEGER PRIMARY KEY,
                    nickname TEXT NOT NULL,
                    remark TEXT,
                    state INTEGER,
                    is_delete INTEGER,
                    last_update INTEGER NOT NULL
                    );)";
    if(!query.exec(sql_friends)){
        qDebug()<<"创建好友表失败";
        return false;
    }

    //创建消息表
    QString sql_message=R"(CREATE TABLE IF NOT EXISTS message(
                        message_type  INTEGER NOT NULL,
                        msg_dlgid INTEGER NOT NULL,
                        send_id  INTEGER NOT NULL,
                        content TEXT NOT NULL,
                        send_time DATETIME NOT NULL
                        );)";
    if(!query.exec(sql_message)){
        qDebug()<<"创建消息表失败";
        return false;
    }

    //创建好友请求表
    QString sql_newFriend=R"(CREATE TABLE IF NOT EXISTS newfriend(
                          client_id INTEGER PRIMARY KEY,
                          username TEXT NOT NULL,
                          message TEXT NOT NULL,
                          state INTEGER NOT NULL,
                          last_update DATETIME NOT NULL
                          );)";
    if(!query.exec(sql_newFriend)){
        qDebug()<<"创建好友请求表失败";
        return false;
    }

    //创建好友聊天列表表
    QString sql_chatList=R"(CREATE TABLE IF NOT EXISTS chatlist(
                         friend_id INTEGER PRIMARY KEY,
                         remark TEXT NOT NULL,
                         unread_count INTEGER DEFAULT 0,
                         message TEXT,
                         send_time DATETIME
                         );)";

    if(!query.exec(sql_chatList)){
        qDebug()<<"创建创建好友聊天列表表失败";
        return false;
    }

    qDebug()<<"创建所有表完成";
    return true;
}

void MyDataBase::save_message(int type, QString dlg_id,QString saveId, QString message)
{
    QString sql_string=R"(INSERT INTO message(message_type,msg_dlgid,send_id,content,send_time) VALUES(:message_type,:msg_dlgid,:send_id,:content,CURRENT_TIMESTAMP);)";
    QSqlQuery query(db);
    query.prepare(sql_string);
    query.bindValue(":message_type",type);
    query.bindValue(":msg_dlgid",dlg_id.toInt());
    query.bindValue(":send_id",saveId.toInt());
    query.bindValue(":content",message);
    if(!query.exec()){
        qDebug()<<"保存消息失败"<<query.lastError().text();
        return;
    }
    qDebug()<<"保存消息成功";
}

void MyDataBase::save_chatList(QString clientId, QString name, QDateTime time, QString message, int unreadNum)
{
    QSqlQuery query(db);
    QString select_string=R"(SELECT * FROM chatlist WHERE friend_id=:friend_id;)";
    select_string.replace(":friend_id",clientId);
    QSqlQuery select_query=select_sql(select_string);
    QString sql_string;
    if(select_query.next()){
        sql_string=R"(UPDATE chatlist SET remark=:remark,unread_count=:unread_count,message=:message,send_time=:send_time WHERE friend_id=:friend_id;)";
    }
    else{
        sql_string=R"(INSERT into chatlist(friend_id,remark,unread_count,message,send_time) VALUES(:friend_id,:remark,:unread_count,:message,:send_time);)";
    }
    query.prepare(sql_string);
    query.bindValue(":friend_id",clientId);
    query.bindValue(":remark",name);
    query.bindValue(":unread_count",unreadNum);
    query.bindValue(":message",message);
    query.bindValue(":send_time",time);
    if(!query.exec()){
        qDebug()<<"插入chatlist失败"<<query.lastError().text();
        qDebug()<<"client_id:"<<clientId;
        return;
    }
    qDebug()<<"插入chatlist成功";
    return;
}

void MyDataBase::save_contact(QString friend_id, QString nickname, QString remark, quint8 state, quint8 isdelete, quint64 last_update)
{
    QSqlQuery query(db);
    QString sql_string=R"(INSERT into friendship(friend_id,nickname,remark,state,is_delete,last_update) VALUES(:friend_id,:nickname,:remark,:state,:is_delete,:last_update);)";
    query.prepare(sql_string);
    query.bindValue(":friend_id",friend_id);
    query.bindValue(":nickname",nickname);
    query.bindValue(":remark",remark);
    query.bindValue(":state",state);
    query.bindValue(":is_delete",isdelete);
    query.bindValue(":last_update",last_update);
    if(!query.exec()){
        qDebug()<<"保存用户信息失败"<<query.lastError().text();
    }
    else{
        qDebug()<<"插入成功"<<"id:"<<friend_id;
    }
}

void MyDataBase::save_newFriend(QString friend_id, QString username, QString msg, quint8 state, quint64 last_update)
{
    QString sql_string=R"(INSERT INTO newfriend(client_id,username,message,state,last_update) VALUES(:client_id,:username,:message,:state,:last_update);)";
    QSqlQuery query(db);
    query.prepare(sql_string);
    query.bindValue(":client_id",friend_id);
    query.bindValue(":username",username);
    query.bindValue(":message",msg);
    query.bindValue(":state",state);
    query.bindValue(":last_update",last_update);
    qDebug()<<friend_id<<username<<msg<<state<<last_update;

    if(!query.exec()){
        qDebug()<<"插入新好友失败"<<query.lastError().text();
        qDebug()<<"执行的语句为:"<<query.executedQuery();
    }
    else{
        qDebug()<<"插入新好友成功"<<"friend_id:"<<friend_id;
    }
}

void MyDataBase::update_newFriend(QString client_id,QString username, QString msg, quint8 state, quint64 last_update)
{
    QString sql_string=R"(UPDATE newfriend SET username=:username,message=:message,state=:state,last_update=:last_update  WHERE client_id=:client_id;)";
    QSqlQuery query(db);
    query.prepare(sql_string);
    query.bindValue(":username",username);
    query.bindValue(":message",msg);
    query.bindValue(":state",state);
    query.bindValue(":last_update",last_update);
    query.bindValue(":client_id",client_id);

    if(!query.exec()){
        qDebug()<<"更新update_newFriend失败"<<query.lastError().text();
    }
    else{
        qDebug()<<"更新update_newFriend成功";
    }

}

void MyDataBase::update_contact(QString friend_id, QString nickname, QString remark, quint8 state, quint8 isdelete, quint64 last_update)
{
     QString sql_string=R"(UPDATE friendship SET nickname=:nickname,remark=:remark,state=:state,is_delete=:is_delete,last_update=:last_update WHERE friend_id=:friend_id;)";
     QSqlQuery query(db);
     query.prepare(sql_string);
     query.bindValue(":nickname",nickname);
     query.bindValue(":remark",remark);
     query.bindValue(":state",state);
     query.bindValue(":is_delete",isdelete);
     query.bindValue(":last_update",last_update);
     query.bindValue(":friend_id",friend_id);
     if(!query.exec()){
         qDebug()<<"更新update_contact失败"<<query.lastError().text();
     }else{
         qDebug()<<"更新update_contact成功";
     }
}

QSqlQuery MyDataBase::select_sql(QString sql_string)
{
    qDebug()<<"开始查询";
    QSqlQuery query(db);
    query.prepare(sql_string);
    qDebug()<<"语句准备完成";
    if(!query.exec()){
        qDebug()<<"查询失败"<<query.lastError().text();
        qDebug()<<"执行的语句为:"<<query.lastQuery();
    }
    qDebug()<<"查询结束";
    return query;
}

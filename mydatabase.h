#ifndef MYDATABASE_H
#define MYDATABASE_H

#include <QObject>
#include<QString>
#include<QDateTime>
#include<QSqlDatabase>

class MyDataBase : public QObject
{
    Q_OBJECT
public:
    explicit MyDataBase(QString myId,QObject *parent = nullptr);
    ~MyDataBase();
    bool initDataBase(QString dbName);
    bool creat_database();
    void save_message(int type, QString dlg_id,QString saveId, QString message);
    void save_chatList(QString clientId, QString name, QDateTime time, QString message, int unreadNum);
    void save_contact(QString friend_id,QString nickname,QString remark,quint8 state,quint8 isdelete,quint64 last_update);
    void save_newFriend(QString friend_id,QString username,QString msg,quint8 state,quint64 last_update);
    void update_newFriend(QString client_id,QString username,QString msg,quint8 state,quint64 last_update);
    void update_contact(QString friend_id,QString nickname,QString remark,quint8 state,quint8 isdelete,quint64 last_update);
    QSqlQuery select_sql(QString sql_string);

signals:


private:
    QSqlDatabase db;
    QString my_id;
    QString db_path;
};

#endif // MYDATABASE_H

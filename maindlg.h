#ifndef MAINDLG_H
#define MAINDLG_H

#include <QWidget>
#include<QSize>
#include<QVBoxLayout>
#include<QMap>
#include<QString>
#include<QThread>
#include"chatlistdlg.h"
#include"chatdlg.h"
#include"mytcpsocket.h"
#include"contactsinfodlg.h"
#include"contactslistdlg.h"
#include"newfrienddlg.h"
#include"mydatabase.h"


namespace Ui {
class MainDlg;
}

class MainDlg : public QWidget
{
    Q_OBJECT

public:
    explicit MainDlg(QThread* thread,MyTcpSocket* socket,QString myId,QWidget *parent = nullptr);
    ~MainDlg();
    void checked_contacts();
    void checked_message();
    void showChatDlg(QString clientId,QString name);
    void showContactInfoDlg(QString clientId, QString name);
    void showNewFriendDlg();
    void show_addFriendDlg(QString id,QString name);
    void recv_text(QString id,QString message);
    void recv_fileMessage(QString id,QString fileName,quint64 fileSize);
    void save_message(int type,QString clientId,QString message);
    void save_fileMessage(int type,QString sendId,quint64 fileSize,QString fileName);
    void changeToMessage(QString id,QString remark);
    void request_searchFriend(QString id,QPoint point);
    void reFriend_result(bool isFind,QString client_name);

private slots:
    void on_tbut_close_clicked();
    void on_tbut_mini_clicked();
    void on_tbut_normal_clicked();

protected:
    void showEvent(QShowEvent* event);

signals:
    void message_unRead(QString id,QString message);
    void request_update_contact();
    void request_update_newFriend();

private:
    Ui::MainDlg *ui;
    ChatListDlg* chatlistdlg=nullptr;//聊天消息页面
    ContactsListDlg* contactsListDlg=nullptr;//联系人界面
    NewFriendDlg* newFrientListDlg=nullptr;//好友申请列表
    QVBoxLayout* chatList_layout=nullptr;//中间页面
    QVBoxLayout* chat_layout=nullptr;//右边页面
    QWidget* mid_cur=nullptr;//当前中间页面
    QWidget* right_cur=nullptr;//当前右边页面
    QMap<QString,ChatDlg*> map_chatDlg;
    QMap<QString,ContactsInfoDlg*> map_contactDlg;
    QString my_Id;
    QString reFriend_id;
    QPoint reFriend_point;

    QThread* socket_thread=nullptr;
    MyTcpSocket* mysocket=nullptr;
    static MyDataBase* mainDlg_DB;
};

#endif // MAINDLG_H

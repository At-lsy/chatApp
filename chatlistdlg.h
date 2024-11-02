#ifndef CHATLISTDLG_H
#define CHATLISTDLG_H

#include <QWidget>
#include<QMap>
#include"chatlistitemdlg.h"
#include"mydatabase.h"

namespace Ui {
class ChatListDlg;
}

class ChatListDlg : public QWidget
{
    Q_OBJECT

public:
    explicit ChatListDlg(QWidget *parent = nullptr);
    ~ChatListDlg();
    void add_chatListItem(QString clientId,QString name,QDateTime date,QString message,int unreadNum);
    void read_chatListItem();
    void save_chatListItem();
    void message_unRead(QString id,QString message);
    void text_change(QString clientId,QString text);
    bool client_exist(QString id);
    ChatListItemDlg* get_itemDlg(QString id);

signals:
    void hasCliked(QString clientId,QString name);

private:
    Ui::ChatListDlg *ui;
    QMap<QString,ChatListItemDlg*> map_ChatListItem;
    static MyDataBase* chatlist_DB;
};

#endif // CHATLISTDLG_H

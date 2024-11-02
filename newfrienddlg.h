#ifndef NEWFRIENDDLG_H
#define NEWFRIENDDLG_H

#include <QWidget>
#include<QMap>
#include"mydatabase.h"
#include"newfrientitemdlg.h"

namespace Ui {
class NewFriendDlg;
}

class NewFriendDlg : public QWidget
{
    Q_OBJECT

public:
    explicit NewFriendDlg(QWidget *parent = nullptr);
    ~NewFriendDlg();
    void init();
    void updateItem();
    void read_item();
    void addNewItem(quint16 state,QString id,QString name,QString msg);

signals:
    void agree_addFriend(QString id);
    void recv_agreeFriend(QString id,QString remark);

private:
    Ui::NewFriendDlg *ui;
    QMap<QString,NewFrientItemDlg*> map_itemDlg;
    static MyDataBase* newFriendDlg_DB;
};

#endif // NEWFRIENDDLG_H

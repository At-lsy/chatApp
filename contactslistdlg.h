#ifndef CONTACTSLISTDLG_H
#define CONTACTSLISTDLG_H

#include <QWidget>
#include<QEvent>
#include<QMap>
#include"searchfriendlistdlg.h"
#include"contactslistitemdlg.h"
#include"mydatabase.h"

namespace Ui {
class ContactsListDlg;
}

class ContactsListDlg : public QWidget
{
    Q_OBJECT

public:
    explicit ContactsListDlg(QWidget *parent = nullptr);
    ~ContactsListDlg();
    void init();
    void checked_lineEdit();
    void unchecked_lineEdit();
    void add_contactItem(QString id,QString remark);
    void read_contacts();
    void update_contact();
    void delete_allItem();
    bool exist_client(QString id);
    QString get_remark(QString id);

signals:
    void hasClicked(QString id,QString name);
    void addFriend(QString id,QPoint point);

private:
    Ui::ContactsListDlg *ui;
    SearchFriendListDlg* fList=nullptr;
    QMap<QString,ContactsListItemDlg*> map_itemDlg;
    static MyDataBase* ContactList_DB;
};

#endif // CONTACTSLISTDLG_H

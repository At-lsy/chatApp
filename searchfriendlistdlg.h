#ifndef SEARCHFRIENDLISTDLG_H
#define SEARCHFRIENDLISTDLG_H

#include <QWidget>
#include<QString>
#include<QListWidgetItem>
#include"searchfriendlistitemdlg.h"

namespace Ui {
class SearchFriendListDlg;
}

class SearchFriendListDlg : public QWidget
{
    Q_OBJECT

public:
    explicit SearchFriendListDlg(QWidget *parent = nullptr);
    ~SearchFriendListDlg();
    void search_friend(QString id);
    QWidget* get_widget();

signals:
    void addFriend(QString id,QPoint point);

private:
    Ui::SearchFriendListDlg *ui;
    SearchFriendListItemDlg* fItem=nullptr;
    QListWidgetItem* listItem=nullptr;
};

#endif // SEARCHFRIENDLISTDLG_H

#ifndef SEARCHFRIENDLISTITEMDLG_H
#define SEARCHFRIENDLISTITEMDLG_H

#include <QWidget>
#include<QString>

namespace Ui {
class SearchFriendListItemDlg;
}

class SearchFriendListItemDlg : public QWidget
{
    Q_OBJECT

public:
    explicit SearchFriendListItemDlg(QWidget *parent = nullptr);
    ~SearchFriendListItemDlg();
    void set_labelString(QString msg);


protected:
    void mousePressEvent(QMouseEvent* event);

signals:
    void search_friend(QString id,QPoint point);

private:
    Ui::SearchFriendListItemDlg *ui;
    QString id;
};

#endif // SEARCHFRIENDLISTITEMDLG_H
